//
//  main
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
#include "pattern_detector.hpp"

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
    std::vector<Stock *> stocks = csv->get_stocks();
    
    // Iterate through all stocks and find any 2,2 patterns
    // Assign the patterns vector to each stock's patterns member
    for (std::vector<Stock *>::iterator iter = stocks.begin(); iter != stocks.end(); ++iter) {
        PatternDetector detector(2, 2);
        struct Pattern pattern(2, 2, detector.detect_pattern(*iter));
        (*iter)->add_pattern(pattern);
    }
    
    return 0;
}

/*
 Plans for the pattern detector implementation:
 
 Current Issues:
    - Since it's an object and I only made one instance, everything shares it
    - The object is setup such that whenever it is called, it adds data from all the previous tickers and compares that
 
 Plans to Fix the Issues:
    - Change the stock class such that each stock has a PatternDetector member
    - Change the PatternDetector class such that it simply creates temp vars whenever the detect_pattern function is called
    - As of right now, I'm leaning towards the latter
    - In addition, it should be changed such that you can call the pattern_detector function accepts different pattern lengths and repetitions, rather than initalizing the object with a set pattern.
        - However, in the future, this can cause design issues because you'll likely end up with a set of patterns that I would always go to
        - Change it so that it returns an array of the pattern_detected booleans
 */
