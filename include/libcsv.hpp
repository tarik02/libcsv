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

#ifndef LIBCSV_HPP
#define LIBCSV_HPP

/* LibCSV binding for C++ */

#include "libcsv.h"

#include <deque>
#include <string>
#include <memory>

namespace libcsv {
struct CSVError {
  std::string message;
  size_t line, column;

  CSVError() : message {}, line(0), column(0) {}

  CSVError(std::string message, size_t line, size_t column) :
    message {std::move(message)},
    line(line),
    column(column) {}
};

class CSVColumn {
  friend class CSVRow;
  friend class CSVTable;

private:
  csv_column *column;

public:
  inline CSVColumn() : column {nullptr} {}

private:
  inline CSVColumn(csv_column *column) : column {column} {}

public:
  inline size_t getIndex() const {
    return csv_column_index(column);
  }

  inline const char *getName() const {
    return csv_column_name(column);
  }

  operator bool() const {
    return column != nullptr;
  }
};

class CSVRow {
  friend class CSVColumn;
  friend class CSVTable;

private:
  std::shared_ptr<csv_row> row;

public:
  inline CSVRow() : row {nullptr, csv_row_free} {}

private:
  inline CSVRow(csv_row *row) : row {row, csv_row_free} {}

public:
  inline CSVRow(const CSVRow &row) = default;


  inline size_t getIndex() const {
    return csv_row_index(row.get());
  }

  inline bool isEmpty(const CSVColumn column) const {
    return csv_row_empty(row.get(), column.column);
  }

  inline std::string getValue(const CSVColumn column) const {
    return {csv_row_value(row.get(), column.column)};
  }


  inline bool getValue(const CSVColumn column, int8_t &result) const {
    return csv_row_value_int8(row.get(), column.column, &result);
  }

  inline int8_t getValueOr(const CSVColumn column, int8_t def) const {
    return csv_row_value_int8_default(row.get(), column.column, def);
  }


  inline bool getValue(const CSVColumn column, uint8_t &result) const {
    return csv_row_value_uint8(row.get(), column.column, &result);
  }

  inline uint8_t getValueOr(const CSVColumn column, uint8_t def) const {
    return csv_row_value_uint8_default(row.get(), column.column, def);
  }


  inline bool getValue(const CSVColumn column, int16_t &result) const {
    return csv_row_value_int16(row.get(), column.column, &result);
  }

  inline int16_t getValueOr(const CSVColumn column, int16_t def) const {
    return csv_row_value_int16_default(row.get(), column.column, def);
  }


  inline bool getValue(const CSVColumn column, uint16_t &result) const {
    return csv_row_value_uint16(row.get(), column.column, &result);
  }

  inline uint16_t getValueOr(const CSVColumn column, uint16_t def) const {
    return csv_row_value_uint16_default(row.get(), column.column, def);
  }


  inline bool getValue(const CSVColumn column, int32_t &result) const {
    return csv_row_value_int32(row.get(), column.column, &result);
  }

  inline int32_t getValueOr(const CSVColumn column, int32_t def) const {
    return csv_row_value_int32_default(row.get(), column.column, def);
  }


  inline bool getValue(const CSVColumn column, uint32_t &result) const {
    return csv_row_value_uint32(row.get(), column.column, &result);
  }

  inline uint32_t getValueOr(const CSVColumn column, uint32_t def) const {
    return csv_row_value_uint32_default(row.get(), column.column, def);
  }


  inline bool getValue(const CSVColumn column, int64_t &result) const {
    return csv_row_value_int64(row.get(), column.column, &result);
  }

  inline int64_t getValueOr(const CSVColumn column, int64_t def) const {
    return csv_row_value_int64_default(row.get(), column.column, def);
  }


  inline bool getValue(const CSVColumn column, uint64_t &result) const {
    return csv_row_value_uint64(row.get(), column.column, &result);
  }

  inline uint64_t getValueOr(const CSVColumn column, uint64_t def) const {
    return csv_row_value_uint64_default(row.get(), column.column, def);
  }


  inline bool getValue(const CSVColumn column, float &result) const {
    return csv_row_value_float(row.get(), column.column, &result);
  }

  inline float getValueOr(const CSVColumn column, float def) const {
    return csv_row_value_float_default(row.get(), column.column, def);
  }

  
  inline bool getValue(const CSVColumn column, double &result) const {
    return csv_row_value_double(row.get(), column.column, &result);
  }

  inline double getValueOr(const CSVColumn column, double def) const {
    return csv_row_value_double_default(row.get(), column.column, def);
  }


  operator bool() const {
    return row.operator bool();
  }
};

class CSVTable {
  friend class CSVColumn;
  friend class CSVRow;

private:
  std::shared_ptr<csv_table> table;
  std::deque<CSVError> errors;

public:
  inline CSVTable() : table {csv_table_create(), csv_table_free} {
    if (!table) {
      throw std::bad_alloc {};
    }

    csv_table_set_error_callback(table.get(), &CSVTable::private_error_callback, this);
  }
  inline CSVTable(const CSVTable &) = delete;
  inline CSVTable(CSVTable &&) = default;

private:
  static void private_error_callback(const char *error, size_t line, size_t column, void *data) {
    CSVTable *self = reinterpret_cast<CSVTable *>(data);
    self->errors.emplace_back(error, line, column);
  }

public:
  inline bool hasError() const {
    return !errors.empty();
  }

  inline bool getError(CSVError &error) {
    if (!errors.empty()) {
      error = std::move(errors.front());
      errors.pop_front();
      return true;
    }

    return false;
  }

  inline char getSeparator() const {
    return csv_table_get_separator(table.get());
  }

  inline void setSeparator(char c) {
    csv_table_set_separator(table.get(), c);
  }


  inline void addData(const char *data) {
    csv_table_add_data(table.get(), data);
  }

  inline void addData(const char *data, size_t length) {
    csv_table_add_data_length(table.get(), data, length);
  }

  inline void addData(const std::string &data) {
    csv_table_add_data_length(table.get(), data.c_str(), data.length());
  }


  inline size_t getColumnCount() const {
    return csv_table_column_count(table.get());
  }

  inline CSVColumn getColumn(size_t i) const {
    return csv_table_column(table.get(), i);
  }

  inline CSVColumn getColumn(const char *name) const {
    return csv_table_column_by_name(table.get(), name);
  }

  inline CSVColumn getColumn(const std::string &name) const {
    return csv_table_column_by_name(table.get(), name.c_str());
  }


  inline bool hasHeader() const {
    return csv_table_has_header(table.get());
  }

  inline bool hasRow() const {
    return csv_table_has_row(table.get());
  }

  inline size_t availableRows() const {
    return csv_table_available_rows(table.get());
  }


  inline CSVRow nextRow() {
    return csv_table_next_row(table.get());
  }
};
}

#endif
