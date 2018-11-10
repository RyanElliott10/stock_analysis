//
//  pattern_detector.hpp
//  stock_analysis
//
//  Created by Ryan Elliott on 11/2/18.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#ifndef pattern_detector_hpp
#define pattern_detector_hpp

#include "stock.hpp"

class PatternDetector
{
public:
    PatternDetector(const int pattern_length, const int number_of_repetitions);
    ~PatternDetector();
    std::vector<bool> detect_pattern(Stock *stock);
    
private:
    // Functions
    bool _find_pattern(double price);
    
    // Variables
    int pattern_length_;
    int num_repetitions_;
    int length_;
    int count_;
    double *pattern_arr_;
};

#endif /* pattern_detector_hpp */
