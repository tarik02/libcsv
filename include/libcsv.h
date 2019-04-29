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

#ifndef LIBCSV_H
#define LIBCSV_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#ifndef LIBCSV_DEFAULT_SEPARATOR
#define LIBCSV_DEFAULT_SEPARATOR ','
#endif

#ifndef LIBCSV_INITIAL_TMPSTR_BUFFER
#define LIBCSV_INITIAL_TMPSTR_BUFFER 128
#endif


typedef struct csv_table csv_table;
typedef struct csv_column csv_column;
typedef struct csv_row csv_row;


#ifdef __cplusplus
extern "C" {
#endif


/* Table */
csv_table *csv_table_create();
void csv_table_free(csv_table *table);

char csv_table_get_separator(const csv_table *table);
void csv_table_set_separator(csv_table *table, char separator);

void csv_table_add_data(csv_table *table, const char *data);
void csv_table_add_data_length(csv_table *table, const char *data, size_t length);

size_t csv_table_column_count(const csv_table *table);
csv_column *csv_table_column(const csv_table *table, size_t index);
csv_column *csv_table_column_by_name(const csv_table *table, const char *name);

bool csv_table_has_header(const csv_table *table);
bool csv_table_has_row(const csv_table *table);
size_t csv_table_available_rows(const csv_table *table);

csv_row *csv_table_next_row(csv_table *table);


/* Column */
size_t csv_column_index(const csv_column *column);
const char *csv_column_name(const csv_column *column);


/* Row */
size_t csv_row_index(const csv_row *row);
const char *csv_row_value(const csv_row *row, const csv_column *column);

void csv_row_free(csv_row *row);


#ifdef __cplusplus
}
#endif

#endif
