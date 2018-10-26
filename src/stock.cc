#include <iostream>
#include <string>
#include <vector>

#include "stock.h"
#include "data_point.h"

Stock::Stock() {}

Stock::Stock(std::string ticker)
{
  ticker_ = ticker;
}

Stock::~Stock()
{
  // No *allocs or pointers
}

void Stock::insert_data_point(DataPoint *data)
{
  data_vector_.push_back(data);
}

// Getters
std::string Stock::get_ticker()
{
  return ticker_;
}

std::vector<DataPoint *> Stock::get_data()
{
  return data_vector_;
}
