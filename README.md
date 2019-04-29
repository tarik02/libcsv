## CSV streaming reader library

[![Build Status](https://travis-ci.org/Tarik02/libcsv.svg?branch=master)](https://travis-ci.org/Tarik02/libcsv)
[![codecov](https://codecov.io/gh/Tarik02/libcsv/branch/master/graph/badge.svg)](https://codecov.io/gh/Tarik02/libcsv)

## Installation
You can install library in few ways:
 - Compile and install with cmake:
```bash
$ git clone https://github.com/Tarik02/libcsv.git
$ cd libcsv
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . --target install
```
(After this, you can use it with `find_package(LibCSV)` and adding it as `target_link_libraries` in CMake)
 - Copy `libcsv.h`, `libcsv.hpp` and `libcsv.c` to your project.
 - Include library as git submodule or just copy to your project and add it to your CMake project.

## Bindings
Library is written in C, but it has OOP-style binding for C++ (file `libcsv.hpp`).

## Usage
[simple.c](examples/simple.c)
```c
#include <libcsv.h>

#include <stdio.h>
#include <string.h>


// taxables.csv from https://people.sc.fsu.edu/~jburkardt/data/csv/csv.html
const char csv_source[] = \
"\"Index\", \"Item\", \"Cost\", \"Tax\", \"Total\"\n"
" 1, \"Fruit of the Loom Girl's Socks\",  7.97, 0.60,  8.57\n"
" 2, \"Rawlings Little League Baseball\", 2.97, 0.22,  3.19\n"
" 3, \"Secret Antiperspirant\",           1.29, 0.10,  1.39\n"
" 4, \"Deadpool DVD\",                   14.96, 1.12, 16.08\n"
" 5, \"Maxwell House Coffee 28 oz\",      7.28, 0.55,  7.83\n"
" 6, \"Banana Boat Sunscreen, 8 oz\",     6.68, 0.50,  7.18\n"
" 7, \"Wrench Set, 18 pieces\",          10.00, 0.75, 10.75\n"
" 8, \"M and M, 42 oz\",                  8.98, 0.67,  9.65\n"
" 9, \"Bertoli Alfredo Sauce\",           2.12, 0.16,  2.28\n"
"10, \"Large Paperclips, 10 boxes\",      6.19, 0.46,  6.65\n"
;

int main() {
  csv_table *table = csv_table_create();
  csv_table_add_data_length(table, csv_source, sizeof(csv_source));

  csv_column *c_index = csv_table_column_by_name(table, "Index");
  csv_column *c_item = csv_table_column_by_name(table, "Item");
  csv_column *c_cost = csv_table_column_by_name(table, "Cost");
  csv_column *c_tax = csv_table_column_by_name(table, "Tax");
  csv_column *c_total = csv_table_column_by_name(table, "Total");

  csv_row *row;
  int i = 0;
  while ((row = csv_table_next_row(table))) {
    printf("Taxable #%d:\n", ++i);
    printf("\tIndex: %s\n", csv_row_value(row, c_index));
    printf("\tItem: %s\n", csv_row_value(row, c_item));
    printf("\tCost: %s\n", csv_row_value(row, c_cost));
    printf("\tTax: %s\n", csv_row_value(row, c_tax));
    printf("\tTotal: %s\n", csv_row_value(row, c_total));
    printf("\n");

    csv_row_free(row);
  }

  csv_table_free(table);

  return 0;
}
```
