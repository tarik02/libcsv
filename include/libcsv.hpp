// MIT License
// 
// Copyright (c) 2019 Tarik02
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

#ifndef LIBCSV_HPP
#define LIBCSV_HPP

// LibCSV binding for C++

#include "libcsv.h"

#include <string>
#include <memory>

namespace libcsv {
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

  inline const char *getValue(const CSVColumn column) const {
    return csv_row_value(row.get(), column.column);
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

public:
  inline CSVTable() : table {csv_table_create(), csv_table_free} {
    if (!table) {
      throw std::bad_alloc {};
    }
  }
  inline CSVTable(std::shared_ptr<csv_table> table) : table {table} {}


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

#endif // LIBCSV_HPP
