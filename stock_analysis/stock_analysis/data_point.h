#ifndef _H_DATA_POINT
#define _H_DATA_POINT

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
  std::string date_;
  long volume_;
  float open_, adj_close_, low_, high_;
};

#endif
