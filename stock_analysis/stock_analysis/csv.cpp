//
//  csv.cpp
//  stock_analysis
//
//  Created by Ryan Elliott on 10/30/18.
//  Copyright © 2018 Ryan Elliott. All rights reserved.
//

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <typeinfo>
#include <sqlite3.h>

#include "csv.hpp"
#include "stock.hpp"
#include "data_point.hpp"

CSV::CSV(const std::string data_path, const std::string db_path)
{
    enter_data(data_path);
    db_path_ = db_path;
}

CSV::~CSV()
{
    for (int i = 0; i < stocks_.size(); i++) {
        free(stocks_.at(i));
    }
}

/**
 Getter for stocks_ vector

 @return stocks_ vector
 */
std::vector<Stock *> CSV::get_stocks()
{
    return stocks_;
}

/**
 Accepts a string containing the target directory, changes the cwd to the correct directory, dumps all CSV files into the csv_filenames_ member, and enters the data into a SQLite3 database.

 @param target_dir Directory containg all CSV files with data
 @return Boolean containing success value
 */
bool CSV::enter_data(const std::string target_dir)
{
    // Change cwd to dir with all the .csv files
    char path[FILENAME_MAX];
    DIR *dir;
    char init_dir[1024];
    getcwd(init_dir, sizeof(init_dir));
    
    memset(path, FILENAME_MAX, 0);
    chdir(target_dir.c_str());
    if ((dir = opendir(".")) != NULL) {
        _get_csvs(dir);
    } else {
        std::cerr << "Unable to open directory" << std::endl;
        chdir("../");
        return false;
    }
    
    _parse_data();
    chdir(init_dir);
    
    return true;
}

/**
 Iterates through all Stocks, and their subsequent DataPoint objects, and checks if they should be added to the database.

 @return Boolean containing success value
 */
bool CSV::update_db()
{
    sqlite3 *db;
    char *db_error_msg;
    int status;
    std::string sql;
    
    // This DB will have n tables, where n is equal to the amount of tickers.
    // Essentially, each ticker has its own database with all of its historical
    // data.

    // Open database
    
    sql = db_path_;
    status = sqlite3_open(sql.c_str(), &db);
    if (status != SQLITE_OK) {
        std::cerr << "Unable to open database" << std::endl;
        return false;
    }
    
    int sum = 0;
    int i = 0;
    
    std::vector<Stock *>::iterator st_iter;
    std::cout << "Entering historical data into db" << std::endl;
    
    // Wrap the SQL in a transaction to increase performance by ~200x
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &db_error_msg);
    for (st_iter = stocks_.begin(); st_iter != stocks_.end(); st_iter++) {
        std::string ticker = (*st_iter)->get_ticker();
        
        // Create table
        sql = "CREATE TABLE IF NOT EXISTS \"";
        sql.append(ticker);
        sql.append("\"(ticker TEXT, date TEXT, volume INTEGER, open REAL, adj_close REAL, low REAL, high REAL, UNIQUE(date))");
        
        _execute_sql(db, sql.c_str(), _callback, 0, &db_error_msg);
        
        sum += (*st_iter)->get_data().size();
        for (int i = 0; i < (*st_iter)->get_data().size(); i++) {
            DataPoint *curr_dp = (*st_iter)->get_data().at(i);
             // Insert data into database
            sql = "INSERT INTO \"" + ticker + "\" VALUES ('" + ticker + "', '";
            sql.append(curr_dp->get_date());
            sql.append("', ");
            sql.append(std::to_string(curr_dp->get_volume()));
            sql.append(", ");
            sql.append(std::to_string(curr_dp->get_open()));
            sql.append(", ");
            sql.append(std::to_string(curr_dp->get_adj_close()));
            sql.append(", ");
            sql.append(std::to_string(curr_dp->get_low()));
            sql.append(", ");
            sql.append(std::to_string(curr_dp->get_high()));
            sql.append(")");
            
            int status = _execute_sql(db, sql.c_str(), _callback, 0, &db_error_msg);
            if (status == 1) {
                // If it tried to enter duplicate data
                break;
            }
            if (status == 2) {
                // Catastrophic SQL insertion error
                std::cout << "Catastrophic SQL insertion error, exiting and closing db" << std::endl;
                return false;
            }
        }
        
        _show_progress(stocks_.size(), ++i, std::string("Data Insertion Progress: "));
    }
    
    std::cout << std::endl;
    sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &db_error_msg);
    std::cout << "Total data points: " << sum << std::endl;
    sqlite3_close(db);
    
    return true;
}


/**
 Updates the progress bar for iterating through large datasets that might appeas as being hung in the program, when in actuality, they simply contain a few million datapoints and take a while to be iterated through

 @param size The size of the entire vector
 @param curr The current index
 @param label Description of the type of thing being iterated through
 */
void CSV::_show_progress(const unsigned long size, const int curr, std::string label)
{
    // To correctly format the output
    label.resize(50, ' ');
    std::string prog_bar;
    // Has 25 spaces to display # signs
    for (int i = 0; i < 25; i++) {
        if (i < ((float(curr)/size)*25)) {
            prog_bar.append("#");
        } else {
            prog_bar.append(" ");
        }
    }
    
    // To fully clear the console
    std::cout << "\r";
    for (int i = 0; i < 100; ++i) {
        std::cout << " ";
    }
    std::cout << "\r" << label << "[" << prog_bar << "] " << (float(curr)/size*100) << "%" << " (" << curr << " / " << size << ")";
    std::cout.flush();
}

/**
 My own wrapper to execute SQL and do all appropriate error checking

 @param db Pointer to already opened database
 @param sql String containing SQL query to be executed
 @param callback Function used for the callback function
 @return int containing the success value of the function (0 is successful, 1 is duplicate data, 2 is catastrophic failure
 */
int CSV::_execute_sql(sqlite3 *db, const char *sql,
                      int (*callback)(void *, int, char **, char **),
                      void *cb_arg, char **db_error_msg)
{
    sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, db_error_msg);
    int status = sqlite3_exec(db, sql, _callback, cb_arg, db_error_msg);
    
    if (status != SQLITE_OK) {
        if (std::string(*db_error_msg).find("UNIQUE") == 0) {
            return 1;
        } else {
            /**
             * Really should figure out how to handle this catastrophic error
             */
            sqlite3_close(db);
            return 2;
        }
    }
    
    return 0;
}

/**
 Callback function for sqlite_exec

 @param NotUsed N/A
 @param argc N/A
 @param argv N/A
 @param azColName N/A
 @return N/A
 */
int CSV::_callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    std::cout << std::endl;
    return 0;
}

/**
 Accepts a directory, iterates through it, and assigns all CSV files to the csv_filenames_ vector to be used later.

 @param dir Directory containing all CSV files
 */
void CSV::_get_csvs(DIR *dir)
{
    struct dirent *de;
    
    while ((de = readdir(dir)) != NULL) {
        std::string de_name(de->d_name);
        if (de_name.find(".csv") != std::string::npos) {
            csv_filenames_.push_back(de_name);
        }
    }
}

/**
 * I
 */

/**
 Iterates through the csv_filenames_ data, opens each file and pushes the new DataPoint and Stock objects to the appropriate vectors
 */
void CSV::_parse_data()
{
    std::cout << "Parsing data from " << csv_filenames_.size() << " CSV files"
    << std::endl;
    
    for (int i = 0; i < csv_filenames_.size(); i++) {
        const std::string input_file(csv_filenames_.at(i));
        std::ifstream input_stream(input_file);
        std::string data_line;
        std::string ticker(input_file);
        std::string prev_ticker = "";
        ticker = ticker.substr(0, ticker.find(".csv"));
        Stock *stock = new Stock(ticker);
        
        // Iterate through each line in the file
        while (getline(input_stream, data_line)) {
            // Gets vector of data in the current line
            std::vector<std::string> line_data = _parseline(data_line);
            
            // Ensures it got all the data from the line
            if (line_data.size() != 7) {
                continue;
            }
            
            // Cuts csv_filenames_ off to 2 decimal points
            line_data.at(2) = line_data.at(2).substr(0, line_data.at(2).find('.') + 3);
            line_data.at(3) = line_data.at(3).substr(0, line_data.at(3).find('.') + 3);
            line_data.at(4) = line_data.at(4).substr(0, line_data.at(4).find('.') + 3);
            line_data.at(5) = line_data.at(5).substr(0, line_data.at(5).find('.') + 3);
            
            // Create new DataPoint object and add to the Stock's historical data
            try {
                DataPoint *dp = new DataPoint(line_data.at(0),
                                              std::stol(line_data.at(1)),
                                              std::stof(line_data.at(2)),
                                              std::stof(line_data.at(5)),
                                              std::stof(line_data.at(3)),
                                              std::stof(line_data.at(4)));
                stock->insert_data_point(dp);
            } catch (std::invalid_argument &ia) {
                if (prev_ticker.compare(ticker) == 0) {
                    std::cerr << stock->get_ticker()
                    << ": Unable to create DataPoint" << std::endl;
                }
                prev_ticker = ticker;
                continue;
            }
        }
        
        // Add the stock (with all the data associated with it) to stocks_ vector
        // if the Stock has DataPoints associated with it
        if (stock->get_data().size() > 0) {
            stocks_.push_back(stock);
        } else {
            free(stock);
        }
        
        // Display counter
        _show_progress(csv_filenames_.size(), i+1, "Parsing Progress: ");
    }
    std::cout << std::endl;
}

/**
 Parses a single line from a CSV file

 @param data_line Line from the CSV file containing all the data
 @return vector containing all the data from the line in string format
 */
std::vector<std::string> CSV::_parseline(std::string data_line)
{
    std::string token;
    std::vector<std::string> data;
    std::istringstream string_stream(data_line);
    
    while (getline(string_stream, token, ',')) {
        data.push_back(token);
    }
    
    return data;
}
