//
//  analysis.cpp
//  stock_analysis
//
//  Created by Ryan Elliott on 10/30/18.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#include <string>
#include <stdio.h>
#include <iostream>

#include "csv.hpp"
#include "stock.hpp"
#include "data_point.hpp"

/**
 * Please note: if you run this on the full_history dataset, it will parse 21.8
 * million datapoints, consuming about 1.5 GB of RAM per execution
 */
int main(int argc, char *argv[])
{
    std::string input;
    CSV *csv;
    
    switch(argc) {
        case 2:
            csv = new CSV(std::string(argv[1]), "./HistoricalData.db");
            break;
        case 3:
            csv = new CSV(std::string(argv[1]), std::string(argv[2]));
            break;
        default:
            std::cout << "Usage: ./analysis [ path to data dir ] [ path to database ] " << std::endl;
            return 0;
    }
    
    csv->update_db();
    
    return 0;
}
