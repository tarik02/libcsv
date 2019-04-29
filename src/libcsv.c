/*
 * MIT License
 * 
 * Copyright (c) 2019 Tarik02
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 */

#include "libcsv.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


enum csv_table_state {
  TABLE_STATE_NEWLINE,

  TABLE_STATE_COLUMN_BEGIN,
  TABLE_STATE_COLUMN_IN,
  TABLE_STATE_COLUMN_IN_ESCAPE,
  TABLE_STATE_COLUMN_IN_ESCAPE_ESCAPE,
  TABLE_STATE_COLUMN_IN_ESCAPE_END,
};

struct csv_table {
  char separator;

  enum csv_table_state state;
  char *state_cs;
  size_t state_cs_len, state_cs_cap;
  csv_row *state_row;
  size_t state_row_column;

  bool has_header;

  size_t columns_count;
  csv_column *columns;

  size_t rows_counter;
  size_t rows_begin;
  size_t rows_end;
  size_t rows_capacity; /* = 2^rows_capacity_log */
  size_t rows_capacity_log; /* = log2 rows_capacity */
  size_t rows_capacity_mask; /* = (~0) >> (sizeof(rows_capacity_mask) * 8 - rows_capacity_log) */
  csv_row **rows_queue;
};

struct csv_column {
  csv_table *table;
  size_t index;
  char *name;
};

struct csv_row {
  csv_table *table;
  size_t index;
  char *values[0];
};


/* Table */
csv_table *csv_table_create() {
  csv_table *table = malloc(sizeof(csv_table));

  table->separator = LIBCSV_DEFAULT_SEPARATOR;

  table->state = TABLE_STATE_COLUMN_BEGIN;
  table->state_cs = NULL;
  table->state_cs_len = table->state_cs_cap = 0;
  table->state_row = NULL;
  table->state_row_column = 0;

  table->has_header = false;

  table->columns_count = 0;
  table->columns = NULL;

  table->rows_counter = 0;
  table->rows_begin = 0;
  table->rows_end = 0;
  table->rows_capacity = 4;
  table->rows_capacity_log = 2;
  table->rows_capacity_mask = 0b11;
  table->rows_queue = malloc(sizeof(table->rows_queue[0]) * table->rows_capacity);

  return table;
}

void csv_table_free(csv_table *table) {
  if (table == NULL) {
    return;
  }

  for (
    size_t i = table->rows_begin, end = table->rows_end, mask = table->rows_capacity_mask;
    i != end;
    ++i, i &= mask
  ) {
    csv_row_free(table->rows_queue[i]);
  }
  free(table->rows_queue);

  for (size_t i = table->columns_count; i --> 0; ) {
    free(table->columns[i].name);
  }
  free(table->columns);

  csv_row_free(table->state_row);

  free(table->state_cs);

  free(table);
}

char csv_table_get_separator(const csv_table *table) {
  return table->separator;
}

void csv_table_set_separator(csv_table *table, char separator) {
  table->separator = separator;
}

void csv_table_add_data(csv_table *table, const char *data) {
  csv_table_add_data_length(table, data, strlen(data));
}

static void csv_table_state_cs_put(csv_table *table, char c) {
  if (table->state_cs_cap == 0) {
    table->state_cs = malloc(LIBCSV_INITIAL_TMPSTR_BUFFER);
    table->state_cs_cap = LIBCSV_INITIAL_TMPSTR_BUFFER;
  }

  if (table->state_cs_len >= table->state_cs_cap) {
    table->state_cs_cap *= 2;
    table->state_cs = realloc(table->state_cs, table->state_cs_cap);
  }

  table->state_cs[table->state_cs_len] = c;
  ++table->state_cs_len;
}

static void csv_table_state_cs_flush(csv_table *table) {
  char *str = malloc(table->state_cs_len + 1);
  memcpy(str, table->state_cs, table->state_cs_len);
  str[table->state_cs_len] = '\0';
  table->state_cs_len = 0;

  if (!table->has_header) {
    table->columns = realloc(table->columns, (table->columns_count + 1) * sizeof(csv_column));
    csv_column *col = &table->columns[table->columns_count];

    col->table = table;
    col->index = table->columns_count;
    col->name = str;

    ++table->columns_count;
  } else {
    csv_row *state_row = table->state_row;
    if (state_row == NULL) {
      state_row = malloc(sizeof(csv_row) + sizeof(table->state_row->values[0]) * table->columns_count);
      state_row->table = table;
      state_row->index = table->rows_counter;
      for (size_t i = table->columns_count; i --> 0; ) {
        state_row->values[i] = NULL;
      }

      ++table->rows_counter;
      table->state_row = state_row;
      table->state_row_column = 0;
    }

    if (table->state_row_column >= table->columns_count) {
      abort();
    }

    state_row->values[table->state_row_column] = str;
    ++table->state_row_column;
    /* TODO: Fix overflow */
  }
}

static void csv_table_state_flush_row(csv_table *table) {
  if (!table->has_header) {
    table->has_header = true;
  } else if (table->state_row != NULL) {
    if (((table->rows_end + 1) & table->rows_capacity_mask) == table->rows_begin) {
      size_t old_mask = table->rows_capacity_mask;

      table->rows_capacity <<= 1; /* *= 2 */
      ++table->rows_capacity_log;
      table->rows_capacity_mask = (table->rows_capacity_mask << 1) | 1;

      csv_row **new_rows = malloc(sizeof(csv_row *) * table->rows_capacity);
      size_t rows_count = 0;
      for (size_t i = 0, j = table->rows_begin; j != table->rows_end; ++i, ++j, j &= old_mask) {
        new_rows[i] = table->rows_queue[j];
        ++rows_count;
      }

      free(table->rows_queue);
      table->rows_queue = new_rows;
      table->rows_begin = 0;
      table->rows_end = rows_count;
    }

    table->rows_queue[table->rows_end] = table->state_row;
    table->state_row = NULL;
    table->state_row_column = 0;

    ++table->rows_end;
    table->rows_end &= table->rows_capacity_mask;
  }
}

void csv_table_add_data_length(csv_table *table, const char *data, size_t length) {
  const char *begin = data, *end = data + length;
  enum csv_table_state state = table->state;

  while (begin < end) {
    char c = *begin;

    switch (state) {
    case TABLE_STATE_NEWLINE:
      csv_table_state_flush_row(table);
      if (c != '\n' && c != '\r') {
        state = TABLE_STATE_COLUMN_BEGIN;
        --begin;
      }
      break;

    case TABLE_STATE_COLUMN_BEGIN:
      if (c  == ' ' || c == '\t') {
        /* Skip whitespace */
      } else if (c == '\n' || c == '\r') {
        csv_table_state_cs_flush(table);
        state = TABLE_STATE_NEWLINE; /* TODO: Is this needed? */
        --begin;
      } else if (c == '"') {
        state = TABLE_STATE_COLUMN_IN_ESCAPE;
      } else {
        csv_table_state_cs_put(table, c);
        state = TABLE_STATE_COLUMN_IN;
      }
      break;

    case TABLE_STATE_COLUMN_IN:
      if (c == '\n' || c == '\r') {
        csv_table_state_cs_flush(table);
        state = TABLE_STATE_NEWLINE;
        --begin;
      } else if (c == table->separator) {
        csv_table_state_cs_flush(table);
        state = TABLE_STATE_COLUMN_BEGIN;
      } else {
        csv_table_state_cs_put(table, c);
      }
      break;

    case TABLE_STATE_COLUMN_IN_ESCAPE:
      if (c == '"') {
        state = TABLE_STATE_COLUMN_IN_ESCAPE_ESCAPE;
      } else {
        csv_table_state_cs_put(table, c);
      }
      break;

    case TABLE_STATE_COLUMN_IN_ESCAPE_ESCAPE:
      if (c == '"') {
        csv_table_state_cs_put(table, c);
        state = TABLE_STATE_COLUMN_IN_ESCAPE;
      } else {
        state = TABLE_STATE_COLUMN_IN_ESCAPE_END;
        --begin;
      }
      break;

    case TABLE_STATE_COLUMN_IN_ESCAPE_END:
      if (c == '\n' || c == '\r') {
        csv_table_state_cs_flush(table);
        state = TABLE_STATE_NEWLINE;
        --begin;
      } else if (c == table->separator) {
        csv_table_state_cs_flush(table);
        state = TABLE_STATE_COLUMN_BEGIN;
      } else {
        /* TODO: Error */
      }
      break;
    }

    ++begin;
  }

  table->state = state;
}


size_t csv_table_column_count(const csv_table *table) {
  return table->columns_count;
}

csv_column *csv_table_column(const csv_table *table, size_t index) {
  if (index >= table->columns_count) {
    return NULL;
  }

  return &table->columns[index];
}

csv_column *csv_table_column_by_name(const csv_table *table, const char *name) {
  for (size_t i = table->columns_count; i --> 0; ) {
    if (strcmp(table->columns[i].name, name) == 0) {
      return &table->columns[i];
    }
  }

  return NULL;
}


bool csv_table_has_header(const csv_table *table) {
  return table->has_header;
}

bool csv_table_has_row(const csv_table *table) {
  return table->rows_begin != table->rows_end;
}

size_t csv_table_available_rows(const csv_table *table) {
  if (table->rows_begin > table->rows_end) {
    return table->rows_capacity - table->rows_begin + table->rows_end;
  }

  return table->rows_end - table->rows_begin;
}

csv_row *csv_table_next_row(csv_table *table) {
  if (table->rows_begin == table->rows_end) {
    return NULL;
  }

  csv_row *row = table->rows_queue[table->rows_begin];
  ++table->rows_begin;
  table->rows_begin &= table->rows_capacity_mask;

  return row;
}


/* Column */
size_t csv_column_index(const csv_column *column) {
  return column->index;
}

const char *csv_column_name(const csv_column *column) {
  return column->name;
}


/* Row */
size_t csv_row_index(const csv_row *row) {
  return row->index;
}

const char *csv_row_value(const csv_row *row, const csv_column *column) {
  assert(row->table == column->table);

  return row->values[column->index];
}

void csv_row_free(csv_row *row) {
  if (row == NULL) {
    return;
  }

  for (size_t i = row->table->columns_count; i --> 0; ) {
    free(row->values[i]);
  }

  free(row);
}
