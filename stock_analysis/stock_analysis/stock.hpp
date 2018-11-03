//
//  stock.hpp
//  stock_analysis
//
//  Created by Ryan Elliott on 10/30/18.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#ifndef stock_hpp
#define stock_hpp

#include <string>
#include <vector>
#include <stdio.h>

#include "data_point.hpp"

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
    std::vector<DataPoint *> historical_data_;
};


#endif /* stock_hpp */
