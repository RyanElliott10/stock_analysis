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

struct Pattern
{
    const int pattern_length, num_repetitions;
    std::vector<bool> pattern_vector;
    
    Pattern(int pl, int nm, std::vector<bool> pv) : pattern_length(pl), num_repetitions(nm){
        pattern_vector = pv;
    };
};

class Stock
{
public:
    Stock();
    Stock(std::string ticker);
    ~Stock();
    void insert_data_point(DataPoint *data);
    void add_pattern(struct Pattern);
    std::string get_ticker();
    std::vector<DataPoint *> get_data();
    std::vector<struct Pattern> get_patterns();
    
private:
    // Variables
    std::string ticker_;
    std::vector<DataPoint *> historical_data_;
    
    // Functions
    std::vector<struct Pattern> patterns;   // vector containing pattern structs, which contain details about the pattern (pattern type)
//    std::vector<std::vector<bool> > patterns;   // vector containing pattern vectors
};


#endif /* stock_hpp */
