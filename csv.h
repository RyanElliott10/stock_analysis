#ifndef _H_CSV
#define _H_CSV

#include <string>
#include <vector>
#include <dirent.h>

class CSV
{
public:
  CSV();
  ~CSV();
  bool enter_data(std::string dir);

private:
  void _get_csvs(DIR *dir);
  std::vector<std::string> csv_vector_;
};

#endif