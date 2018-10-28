#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include <typeinfo>
#include <sstream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "csv.h"
#include "stock.h"
#include "data_point.h"

CSV::CSV(std::string path)
{
   enter_data(path);
}

CSV::~CSV()
{
   // No *allocs or pointers
}

/**
 * Accepts a string containing the target directory, changes the cwd to the
 * correct directory, dumps all CSV files into the csv_filenames_ member, and
 * enters the data into a SQLite3 database.
 * 
 * Returns boolean of success
 */
bool CSV::enter_data(std::string target_dir)
{
   // change cwd to dir with all the .csv files
   char path[FILENAME_MAX];
   DIR *dir;

   memset(path, FILENAME_MAX, 0);
   chdir(target_dir.c_str());
   if ((dir = opendir(".")) != NULL)
   {
      _get_csvs(dir);
   }
   else
   {
      std::cerr << "Unable to open directory" << std::endl;
      return false;
   }

   _parse_data();
   return true;
}

std::vector<Stock *> CSV::get_stocks()
{
   return stocks_;
}

/**
 * Accepts a directory, iterates through it, and assigns all CSV files to
 * the csv_filenames_ vector to be used later.
 */
void CSV::_get_csvs(DIR *dir)
{
   struct dirent *de;

   while ((de = readdir(dir)) != NULL)
   {
      std::string de_name(de->d_name);
      if (de_name.find(".csv") != std::string::npos)
      {
         csv_filenames_.push_back(de_name);
      }
   }
}

/**
 * Iterates through the csv_filenames_ data, opens each file and creates DataPoint
 * objects for each line.
 */
void CSV::_parse_data()
{
   std::cout << "Parsing data from " << csv_filenames_.size() << " CSV files"
             << std::endl;

   for (int i = 0; i < csv_filenames_.size(); i++)
   {
      const std::string input_file(csv_filenames_.at(i));
      std::ifstream input_stream(input_file);
      std::string data_line;

      // Creates Stock object to do operations on. The if is just for testing
      // purposes, remove once fully implemented
      std::string ticker(input_file);
      std::string prev_ticker = "";
      ticker = ticker.substr(0, ticker.find(".csv"));
      Stock *stock = new Stock(ticker);

      // Iterate through each line in the file
      while (getline(input_stream, data_line))
      {
         // Gets vector of data in the current line
         std::vector<std::string> line_data = _parseline(data_line);

         // Ensures it got all the data from the line
         if (line_data.size() != 7)
         {
            continue;
         }

         // Cuts csv_filenames_ off to 2 decimal points
         // std::cout << "Here: " << line_data.size() << std::endl;
         line_data.at(2) = line_data.at(2).substr(0, line_data.at(2).find('.') + 3);
         line_data.at(3) = line_data.at(3).substr(0, line_data.at(3).find('.') + 3);
         line_data.at(4) = line_data.at(4).substr(0, line_data.at(4).find('.') + 3);
         line_data.at(5) = line_data.at(5).substr(0, line_data.at(5).find('.') + 3);

         // Create new DataPoint object and add to the Stock's historical data
         try
         {
            DataPoint *dp = new DataPoint(line_data.at(0),
                                          std::stol(line_data.at(1)),
                                          std::stof(line_data.at(2)),
                                          std::stof(line_data.at(5)),
                                          std::stof(line_data.at(3)),
                                          std::stof(line_data.at(4)));
            stock->insert_data_point(dp);
         }
         catch (std::invalid_argument &ia)
         {
            if (prev_ticker.compare(ticker) == 0)
            {
               std::cerr << stock->get_ticker() << ": Unable to create DataPoint"
                         << std::endl;
            }
            prev_ticker = ticker;
            continue;
         }
      }

      // Add the stock (with all the data associated with it) to stocks_ vector
      // if the Stock has DataPoints associated with it
      if (stock->get_data().size() > 0)
      {
         stocks_.push_back(stock);
      }
      else
      {
         free(stock);
      }

      // Display counter
      std::cout << "\r" << i + 1 << " / " << csv_filenames_.size();
      std::cout.flush();
   }

   std::cout << std::endl;
}

std::vector<std::string> CSV::_parseline(std::string data_line)
{
   std::string token;
   std::vector<std::string> data;
   std::istringstream string_stream(data_line);

   while (getline(string_stream, token, ','))
   {
      data.push_back(token);
   }

   return data;
}