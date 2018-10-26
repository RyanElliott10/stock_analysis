#include <string>
#include <iostream>
#include <vector>

#include "data_point.h"

DataPoint::DataPoint() {}

DataPoint::DataPoint(int volume, float open, float adj_close,
                     float low, float high)
{
  volume_ = volume;
  open_ = open;
  adj_close_ = adj_close;
  low_ = low;
  high_ = high;
}

DataPoint::~DataPoint()
{
  // No *allocs or pointers
}

int DataPoint::get_volume()
{
  return volume_;
}

float DataPoint::get_open()
{
  return open_;
}

float DataPoint::get_adj_close()
{
  return adj_close_;
}

float DataPoint::get_low()
{
  return low_;
}

float DataPoint::get_high()
{
  return high_;
}