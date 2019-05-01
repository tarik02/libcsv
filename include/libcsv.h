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

typedef void (*csv_error_callback)(const char *error, size_t line, size_t column, void *data);


#ifdef __cplusplus
extern "C" {
#endif


/* Table */
csv_table *csv_table_create();
void csv_table_free(csv_table *table);

void csv_table_set_error_callback(csv_table *table, csv_error_callback error_callback, void *data);

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

bool csv_row_empty(const csv_row *row, const csv_column *column);

const char *csv_row_value(const csv_row *row, const csv_column *column);
const char *csv_row_value_default(const csv_row *row, const csv_column *column, const char *def);

bool csv_row_value_int8(const csv_row *row, const csv_column *column, int8_t *result);
int8_t csv_row_value_int8_default(const csv_row *row, const csv_column *column, int8_t def);

bool csv_row_value_uint8(const csv_row *row, const csv_column *column, uint8_t *result);
uint8_t csv_row_value_uint8_default(const csv_row *row, const csv_column *column, uint8_t def);

bool csv_row_value_int16(const csv_row *row, const csv_column *column, int16_t *result);
int16_t csv_row_value_int16_default(const csv_row *row, const csv_column *column, int16_t def);

bool csv_row_value_uint16(const csv_row *row, const csv_column *column, uint16_t *result);
uint16_t csv_row_value_uint16_default(const csv_row *row, const csv_column *column, uint16_t def);

bool csv_row_value_int32(const csv_row *row, const csv_column *column, int32_t *result);
int32_t csv_row_value_int32_default(const csv_row *row, const csv_column *column, int32_t def);

bool csv_row_value_uint32(const csv_row *row, const csv_column *column, uint32_t *result);
uint32_t csv_row_value_uint32_default(const csv_row *row, const csv_column *column, uint32_t def);

bool csv_row_value_int64(const csv_row *row, const csv_column *column, int64_t *result);
int64_t csv_row_value_int64_default(const csv_row *row, const csv_column *column, int64_t def);

bool csv_row_value_uint64(const csv_row *row, const csv_column *column, uint64_t *result);
uint64_t csv_row_value_uint64_default(const csv_row *row, const csv_column *column, uint64_t def);

bool csv_row_value_float(const csv_row *row, const csv_column *column, float *result);
float csv_row_value_float_default(const csv_row *row, const csv_column *column, float def);

bool csv_row_value_double(const csv_row *row, const csv_column *column, double *result);
double csv_row_value_double_default(const csv_row *row, const csv_column *column, double def);

bool csv_row_value_bool(const csv_row *row, const csv_column *column);
bool csv_row_value_bool_default(const csv_row *row, const csv_column *column, bool def);

void csv_row_free(csv_row *row);


#ifdef __cplusplus
}
#endif

#endif
