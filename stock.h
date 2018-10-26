#ifndef _STOCK_H
#define _STOCK_H

#include <string>
#include <vector>

#include "data_point.h"
#include "stock.h"

class Stock
{
public:
  Stock();
  Stock(std::string ticker);
  ~Stock();
  void insert_data_point(DataPoint *data);
  std::string get_ticker();
  std::vector<DataPoint *> get_data();

private:
  std::string ticker_;
  // vector of pointers to DataPoint objects
  std::vector<DataPoint *> data_vector_;
};

#endif