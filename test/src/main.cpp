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

#include <gtest/gtest.h>
#include <libcsv.hpp>

#include <fstream>
#include <string>
#include <random>

using namespace std;
using namespace libcsv;


#define $

static string mlb_players;


TEST(CSVTable, custom_separator) {
  CSVTable table;

  table.setSeparator('|');
  table.addData("Col1|Col2|Col3\n");
  table.addData("Data1.1|Data1.2|Data1.3\n");
  table.addData("Data2.1|Data2.2|Data2.3\n");
  table.addData("Data3.1|Data3.2|Data3.3\n");
  table.addData("Data4.1|Data4.2|Data4.3\n");

  $ ASSERT_TRUE(table.hasHeader());
  $ ASSERT_TRUE(table.hasRow());
  $ ASSERT_EQ(table.getColumnCount(), 3);
  $ ASSERT_EQ(table.availableRows(), 4);


  CSVColumn c_col1 = table.getColumn("Col1");
  $ ASSERT_TRUE(c_col1);
  $ ASSERT_EQ(c_col1.getIndex(), 0);
  $ ASSERT_STREQ(c_col1.getName(), "Col1");

  CSVColumn c_col2 = table.getColumn("Col2");
  $ ASSERT_TRUE(c_col2);
  $ ASSERT_EQ(c_col2.getIndex(), 1);
  $ ASSERT_STREQ(c_col2.getName(), "Col2");

  CSVColumn c_col3 = table.getColumn("Col3");
  $ ASSERT_TRUE(c_col3);
  $ ASSERT_EQ(c_col3.getIndex(), 2);
  $ ASSERT_STREQ(c_col3.getName(), "Col3");


  CSVRow row;

  row = table.nextRow();
  $ ASSERT_EQ(table.availableRows(), 3);
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_col1), "Data1.1");
  $ ASSERT_STREQ(row.getValue(c_col2), "Data1.2");
  $ ASSERT_STREQ(row.getValue(c_col3), "Data1.3");

  row = table.nextRow();
  $ ASSERT_EQ(table.availableRows(), 2);
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_col1), "Data2.1");
  $ ASSERT_STREQ(row.getValue(c_col2), "Data2.2");
  $ ASSERT_STREQ(row.getValue(c_col3), "Data2.3");

  row = table.nextRow();
  $ ASSERT_EQ(table.availableRows(), 1);
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_col1), "Data3.1");
  $ ASSERT_STREQ(row.getValue(c_col2), "Data3.2");
  $ ASSERT_STREQ(row.getValue(c_col3), "Data3.3");

  row = table.nextRow();
  $ ASSERT_EQ(table.availableRows(), 0);
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_col1), "Data4.1");
  $ ASSERT_STREQ(row.getValue(c_col2), "Data4.2");
  $ ASSERT_STREQ(row.getValue(c_col3), "Data4.3");

  $ ASSERT_FALSE(table.hasError());
}

TEST(CSVTable, escaping) {
  CSVTable table;

  table.addData("Col1,Col2,Col3\n");
  table.addData("Value1,Value2,Value3\n");
  table.addData("Value1,\"Value2\",Value3\n");
  table.addData("Value1, \"Value2\",Value3\n");
  table.addData("Value1,\"Value2\" ,Value3\n");
  table.addData("Value1, \"Value2\" ,Value3\n");
  table.addData("Value1,  \"Value2\"  ,Value3\n");
  table.addData("Value1, Value2 ,Value3\n");
  table.addData("Value1,\" Value2 \",Value3\n");
  table.addData("Value1, \" Value2 \",Value3\n");
  table.addData("Value1,\" Value2 \" ,Value3\n");

  $ ASSERT_TRUE(table.hasHeader());
  $ ASSERT_TRUE(table.hasRow());
  $ ASSERT_EQ(table.getColumnCount(), 3);


  CSVColumn c_col1 = table.getColumn("Col1");
  $ ASSERT_TRUE(c_col1);
  $ ASSERT_EQ(c_col1.getIndex(), 0);
  $ ASSERT_STREQ(c_col1.getName(), "Col1");

  CSVColumn c_col2 = table.getColumn("Col2");
  $ ASSERT_TRUE(c_col2);
  $ ASSERT_EQ(c_col2.getIndex(), 1);
  $ ASSERT_STREQ(c_col2.getName(), "Col2");

  CSVColumn c_col3 = table.getColumn("Col3");
  $ ASSERT_TRUE(c_col3);
  $ ASSERT_EQ(c_col3.getIndex(), 2);
  $ ASSERT_STREQ(c_col3.getName(), "Col3");


  CSVRow row;

  row = table.nextRow();
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_col2), "Value2");

  row = table.nextRow();
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_col2), "Value2");

  row = table.nextRow();
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_col2), "Value2");

  row = table.nextRow();
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_col2), "Value2");

  row = table.nextRow();
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_col2), "Value2");

  row = table.nextRow();
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_col2), "Value2");

  row = table.nextRow();
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_col2), "Value2");


  row = table.nextRow();
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_col2), " Value2 ");

  row = table.nextRow();
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_col2), " Value2 ");

  row = table.nextRow();
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_col2), " Value2 ");


  row = table.nextRow();
  $ ASSERT_FALSE(row);

  $ ASSERT_FALSE(table.hasError());
}

TEST(CSVTable, error_handling) {
  CSVTable table;
  CSVError error;


  table.addData("Col1,Col2,\"Col3\" a\n");
  $ ASSERT_TRUE(table.getError(error));
  $ ASSERT_FALSE(table.hasError());
  $ ASSERT_EQ(error.message, "Unexpected symbol after end of escaped string");
  $ ASSERT_EQ(error.line, 1);
  $ ASSERT_EQ(error.column, 17);

  $ ASSERT_TRUE(table.getColumn(2));
  $ ASSERT_STREQ(table.getColumn(2).getName(), "Col3");


  table.addData("Value1, Value2, Value3, Value4WTF\n");
  $ ASSERT_TRUE(table.getError(error));
  $ ASSERT_FALSE(table.hasError());
  $ ASSERT_EQ(error.message, "Unexpected extra column");
  $ ASSERT_EQ(error.line, 2);
  $ ASSERT_EQ(error.column, 24);
}

TEST(CSVTable, chunked_reading) {
  /* Read data fully into one table, and chunked into another one */

  mt19937 rng;
  rng.seed(42);

  CSVTable table1, table2;

  table1.addData(mlb_players);
  $ ASSERT_TRUE(table1.hasHeader());
  $ ASSERT_TRUE(table1.hasRow());
  $ ASSERT_EQ(table1.getColumnCount(), 6);

  CSVColumn c1_name = table1.getColumn("Name");
  $ ASSERT_TRUE(c1_name);

  CSVColumn c1_team = table1.getColumn("Team");
  $ ASSERT_TRUE(c1_team);

  CSVColumn c1_position = table1.getColumn("Position");
  $ ASSERT_TRUE(c1_position);

  CSVColumn c1_height = table1.getColumn("Height(inches)");
  $ ASSERT_TRUE(c1_height);

  CSVColumn c1_weight = table1.getColumn("Weight(lbs)");
  $ ASSERT_TRUE(c1_weight);

  CSVColumn c1_age = table1.getColumn("Age");
  $ ASSERT_TRUE(c1_age);


  size_t remaining = mlb_players.size();
  while (remaining > 0) {
    /* Read by chunks of size 16-64 */
    size_t to_read = min({
      remaining,
      static_cast<size_t>(rng() + 16),
      static_cast<size_t>(64)
    });
    string chunk = mlb_players.substr(mlb_players.size() - remaining, to_read);
    remaining -= to_read;
    table2.addData(chunk);
  }
  $ ASSERT_TRUE(table2.hasHeader());
  $ ASSERT_TRUE(table2.hasRow());
  $ ASSERT_EQ(table2.getColumnCount(), 6);

  CSVColumn c2_name = table2.getColumn("Name");
  $ ASSERT_TRUE(c2_name);

  CSVColumn c2_team = table2.getColumn("Team");
  $ ASSERT_TRUE(c2_team);

  CSVColumn c2_position = table2.getColumn("Position");
  $ ASSERT_TRUE(c2_position);

  CSVColumn c2_height = table2.getColumn("Height(inches)");
  $ ASSERT_TRUE(c2_height);

  CSVColumn c2_weight = table2.getColumn("Weight(lbs)");
  $ ASSERT_TRUE(c2_weight);

  CSVColumn c2_age = table2.getColumn("Age");
  $ ASSERT_TRUE(c2_age);


  $ ASSERT_EQ(table1.availableRows(), table2.availableRows());


  CSVRow row1, row2;
  while ((row1 = table1.nextRow()) && (row2 = table2.nextRow())) {
    $ ASSERT_STREQ(row1.getValue(c1_name), row2.getValue(c2_name));
    $ ASSERT_STREQ(row1.getValue(c1_team), row2.getValue(c2_team));
    $ ASSERT_STREQ(row1.getValue(c1_position), row2.getValue(c2_position));
    $ ASSERT_STREQ(row1.getValue(c1_height), row2.getValue(c2_height));
    $ ASSERT_STREQ(row1.getValue(c1_weight), row2.getValue(c2_weight));
    $ ASSERT_STREQ(row1.getValue(c1_age), row2.getValue(c2_age));
  }


  $ ASSERT_FALSE(table1.hasRow());
  $ ASSERT_FALSE(table2.hasRow());

  $ ASSERT_FALSE(table1.hasError());
  $ ASSERT_FALSE(table2.hasError());
}

TEST(CSVTable, streaming) {
  CSVTable table;

  $ ASSERT_FALSE(table.hasHeader());
  table.addData("index,col1,col2,col3\n");
  $ ASSERT_TRUE(table.hasHeader());


  CSVColumn c_index = table.getColumn("index");
  $ ASSERT_TRUE(c_index);
  $ ASSERT_EQ(c_index.getIndex(), 0);
  $ ASSERT_STREQ(c_index.getName(), "index");

  CSVColumn c_col1 = table.getColumn("col1");
  $ ASSERT_TRUE(c_col1);
  $ ASSERT_EQ(c_col1.getIndex(), 1);
  $ ASSERT_STREQ(c_col1.getName(), "col1");

  CSVColumn c_col2 = table.getColumn("col2");
  $ ASSERT_TRUE(c_col2);
  $ ASSERT_EQ(c_col2.getIndex(), 2);
  $ ASSERT_STREQ(c_col2.getName(), "col2");

  CSVColumn c_col3 = table.getColumn("col3");
  $ ASSERT_TRUE(c_col3);
  $ ASSERT_EQ(c_col3.getIndex(), 3);
  $ ASSERT_STREQ(c_col3.getName(), "col3");

  $ ASSERT_FALSE(table.getColumn("not_exists"));


  $ ASSERT_FALSE(table.hasRow());
  $ ASSERT_EQ(table.availableRows(), 0);
  table.addData("1,v1,v2,v3\n");
  $ ASSERT_EQ(table.availableRows(), 1);
  $ ASSERT_TRUE(table.hasRow());

  table.addData("2,v4,v5,v6\n");
  $ ASSERT_EQ(table.availableRows(), 2);

  table.addData("3,v7,v8,v9\n");
  $ ASSERT_EQ(table.availableRows(), 3);


  CSVRow row;

  row = table.nextRow();
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_index), "1");
  $ ASSERT_STREQ(row.getValue(c_col1), "v1");
  $ ASSERT_STREQ(row.getValue(c_col2), "v2");
  $ ASSERT_STREQ(row.getValue(c_col3), "v3");

  row = table.nextRow();
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_index), "2");
  $ ASSERT_STREQ(row.getValue(c_col1), "v4");
  $ ASSERT_STREQ(row.getValue(c_col2), "v5");
  $ ASSERT_STREQ(row.getValue(c_col3), "v6");

  row = table.nextRow();
  $ ASSERT_TRUE(row);
  $ ASSERT_STREQ(row.getValue(c_index), "3");
  $ ASSERT_STREQ(row.getValue(c_col1), "v7");
  $ ASSERT_STREQ(row.getValue(c_col2), "v8");
  $ ASSERT_STREQ(row.getValue(c_col3), "v9");

  row = table.nextRow();
  $ ASSERT_FALSE(row);

  $ ASSERT_FALSE(table.hasError());
}

TEST(CSVTable, free_nullptr) {
  $ ASSERT_NO_FATAL_FAILURE(csv_table_free(nullptr));
}


TEST(CSVRow, free_nullptr) {
  $ ASSERT_NO_FATAL_FAILURE(csv_row_free(nullptr));
}


void read_file(string name, string &out) {
  ifstream stream(move(name));

  stream.seekg(0, ios::end);   
  out.reserve(stream.tellg());
  stream.seekg(0, ios::beg);

  out.assign(istreambuf_iterator<char>(stream), istreambuf_iterator<char>());
}

int main(int argc, char **argv) {
  string data_path = getenv("TEST_DATA_PATH") ? getenv("TEST_DATA_PATH") : "";
  if (data_path.empty()) {
    data_path = __FILE__;
    data_path = data_path.substr(0, data_path.find_last_of("/\\"));
    data_path += "/../data";
  }

  read_file(data_path + "/mlb_players.csv", mlb_players);

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
