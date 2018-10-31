//
//  stock.cpp
//  stock_analysis
//
//  Created by Ryan Elliott on 10/30/18.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#include <string>
#include <vector>
#include <iostream>

#include "stock.hpp"
#include "data_point.hpp"

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
    historical_data_.push_back(data);
}

// Getters
std::string Stock::get_ticker()
{
    return ticker_;
}

std::vector<DataPoint *> Stock::get_data()
{
    return historical_data_;
}
