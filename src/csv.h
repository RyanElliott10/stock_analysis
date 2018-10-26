#ifndef _H_CSV
#define _H_CSV

#include <string>
#include <vector>
#include <dirent.h>

class CSV
{
public:
  CSV();
  CSV(std::string path);
  ~CSV();
  bool enter_data(std::string dir);

private:
  void _get_csvs(DIR *dir);
  void _parse_data();
  std::vector<std::string> csv_vector_;
};

#endif
