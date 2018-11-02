//
//  csv.hpp
//  stock_analysis
//
//  Created by Ryan Elliott on 10/30/18.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#ifndef csv_hpp
#define csv_hpp

#include <string>
#include <vector>
#include <stdio.h>
#include <dirent.h>
#include <sqlite3.h>

#include "stock.hpp"

class CSV
{
public:
    // Functions
    CSV(const std::string data_path, const std::string db_path);
    ~CSV();
    bool enter_data(const std::string dir);
    bool update_db();
    std::vector<Stock *> get_stocks();
    
private:
    // Variables
    std::vector<std::string> csv_filenames_;
    std::vector<Stock *> stocks_;
    std::string db_path_;
    
    // Functions
    void _show_progress(const unsigned long size, const int curr, std::string label);
    void _get_csvs(DIR *dir);
    void _parse_data();
    std::vector<std::string> _parseline(std::string data_line);
    static int _callback(void *NotUsed, int argc, char **argv, char **azColName);
    int _execute_sql(sqlite3 *db, const char *str,
                     int (*callback)(void *, int, char **, char **),
                     void *cb_arg, char **db_error_msg);
};

#endif /* csv_hpp */
