#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <fstream>

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
 * correct directory, dumps all CSV files into the csv_vector_ member, and
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

/**
 * Accepts a directory, iterates through it, and assigns all CSV files to
 * the csv_vector_ vector to be used later.
 */
void CSV::_get_csvs(DIR *dir)
{
   struct dirent *de;

   while ((de = readdir(dir)) != NULL)
   {
      std::string de_name(de->d_name);
      if (de_name.find(".csv") != std::string::npos)
      {
         csv_vector_.push_back(de_name);
      }
   }
}

/**
 * Iterates through the csv_vector_ data, opens each file and creates DataPoint
 * objects for each line.
 */
void CSV::_parse_data()
{
   int count = 0;
   std::string prev_ticker;
   std::cout << "Parsing data from " << csv_vector_.size()
             << " CSV files" << std::endl;

   for (std::vector<std::string>::iterator curr_file = csv_vector_.begin();
        curr_file != csv_vector_.end(); ++curr_file)
   {
      std::string data_line;
      std::fstream my_file;
      my_file.open(*curr_file);

      // Creates Stock object to do operations on. The if is just for testing
      // purposes, remove once fully implemented
      // if (!std::string(*curr_file).compare("AAPL.csv"))
      // {
      std::string ticker(*curr_file);
      prev_ticker = "";
      ticker = ticker.substr(0, ticker.find(".csv"));
      Stock stock(ticker);

      while (getline(my_file, data_line))
      {
         std::string date, volume, open, close, high, low, adj_close;

         // Get data from the current line CSV file
         getline(my_file, date, ',');
         getline(my_file, volume, ',');
         getline(my_file, open, ',');
         getline(my_file, close, ',');
         getline(my_file, high, ',');
         getline(my_file, low, ',');
         getline(my_file, adj_close, ',');

         // Cuts data off to 2 decimal points
         open = open.substr(0, open.find('.') + 3);
         high = high.substr(0, high.find('.') + 3);
         low = low.substr(0, low.find('.') + 3);
         adj_close = adj_close.substr(0, adj_close.find('.') + 3);

         try
         {
            DataPoint *data = new DataPoint(date, std::stol(volume),
                                            std::stof(open),
                                            std::stof(adj_close),
                                            std::stof(high), std::stof(low));
            stock.insert_data_point(data);
         }
         catch (std::invalid_argument &e)
         {
            if (prev_ticker.compare(ticker) == 0)
            {
               std::cerr << stock.get_ticker() << ": Unable to create DataPoint"
                         << std::endl;
            }
            prev_ticker = ticker;
         }
      }
      // }

      my_file.close();
      std::cout << "\r" << ++count << " / " << csv_vector_.size();

      std::cout.flush();
   }

   std::cout << std::endl;
}