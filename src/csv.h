#ifndef _H_CSV
#define _H_CSV

#include <string>
#include <vector>
#include <dirent.h>
#include <sqlite3.h>

#include "stock.h"

class CSV
{
public:
  CSV(std::string path);
  ~CSV();
  bool enter_data(std::string dir);
  bool update_db();
  std::vector<Stock *> get_stocks();

private:
  void _get_csvs(DIR *dir);
  void _parse_data();
  std::vector<std::string> _parseline(std::string data_line);
  static int _callback(void *NotUsed, int argc, char **argv, char **azColName);
  bool _execute_sql(sqlite3 *db, const char *str,
                    int (*callback)(void *, int, char **, char **),
                    void *cb_arg, char **db_error_msg);

  std::vector<std::string> csv_filenames_;
  std::vector<Stock *> stocks_;
};

#endif
