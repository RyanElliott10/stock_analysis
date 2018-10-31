//
//  data_point.cpp
//  stock_analysis
//
//  Created by Ryan Elliott on 10/30/18.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#include <string>
#include <vector>
#include <iostream>

#include "data_point.hpp"

DataPoint::DataPoint() {}

DataPoint::DataPoint(std::string date, long volume, float open, float adj_close,
                     float low, float high)
{
    date_ = date;
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

// Getters
std::string DataPoint::get_date()
{
    return date_;
}

long DataPoint::get_volume()
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
