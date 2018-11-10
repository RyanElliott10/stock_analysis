//
//  data_point.hpp
//  stock_analysis
//
//  Created by Ryan Elliott on 10/30/18.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#ifndef data_point_hpp
#define data_point_hpp

#include <string>

class DataPoint
{
public:
    DataPoint();
    DataPoint(std::string date, long volume, float open,
              float adj_close, float low, float high);
    ~DataPoint();
    std::string get_date();
    long get_volume();
    float get_open();
    float get_adj_close();
    float get_low();
    float get_high();
    
private:
    // Functions
    
    // Variables
    std::string date_;
    long volume_;
    float open_, adj_close_, low_, high_;
};

#endif /* data_point_hpp */
