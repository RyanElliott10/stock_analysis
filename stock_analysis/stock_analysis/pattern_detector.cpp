//
//  pattern_detector.cpp
//  stock_analysis
//
//  Created by Ryan Elliott on 11/2/18.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#include <string>
#include <vector>
#include <iterator>
#include <iostream>

#include "stock.hpp"
#include "pattern_detector.hpp"

PatternDetector::PatternDetector(const int pattern_length, const int number_of_repetitions)
{
    count_ = 0;
    pattern_length_ = pattern_length;
    num_repetitions_ = number_of_repetitions;
    length_ = pattern_length * num_repetitions_;
    pattern_arr_ = (double *)calloc(length_, sizeof(double));
}

PatternDetector::~PatternDetector()
{
    // free all allocated memory
    free(pattern_arr_);
}

/**
 Accepts a vector of stock pointers, iterates through them, and finds all patterns

 @param stock Stock object
 */
void PatternDetector::detect_pattern(Stock *stock)
{
    std::vector<DataPoint *> data = stock->get_data();
    bool pattern;
    
    for (long j = data.size()-1; j >= 0; j--) {
        pattern = _find_pattern(data.at(j)->get_adj_close());
    }
}

/**
 Find pattern given a stock

 @param price Thing to be compared to
 @return Boolean of whether there has been a pattern found
 */
bool PatternDetector::_find_pattern(double price)
{
    bool pattern_detected = false;
    int index, pattern_count = 0;
    
    // properly index into the array and set the index to that value
    // to ensure the array is formatted correctly
    if (count_ >= length_) {
        for (int i = 0; i < length_-1; i++) {
            pattern_arr_[i] = pattern_arr_[i+1];
        }
        pattern_arr_[length_-1] = price;
        index = length_-1;
    } else {
        index = count_ % length_;
        pattern_arr_[index] = price;
    }
    
    count_++;
    
    // iterate through the array and find if there are any patterns
    for (int i = 0; i < ((length_ < count_) ? length_ : count_); i++) {
        // to skip the first "pattern" to avoid double counting, however, keep it if patternLength = 1
        if (pattern_length_ != 1 && i < pattern_length_) {
            continue;
        }
        
        // if the element we're currently at is equal to the element in the first part of the pattern
        if (pattern_arr_[i % pattern_length_] == pattern_arr_[i]) {
            pattern_count++;
        }
    }
    
    // if there has been at least the correct amount of patterns, set pattern_deted to true
    if (pattern_count >= num_repetitions_) {
        pattern_detected = true;
    }
    
    return pattern_detected;
}
