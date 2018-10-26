#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <dirent.h>

#include "csv.h"

CSV::CSV()
{
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

   return true;
}

/**
 * Accepts a directory, iterates through it, and assigns all CSV files to
 * the csv_vector_ vector to be used later.
 */
void CSV::_get_csvs(DIR *dir)
{
   struct dirent *de;

   if ((dir = opendir(".")) != NULL)
   {
      while ((de = readdir(dir)) != NULL)
      {
         std::string de_name(de->d_name);
         if (de_name.find(".csv") != std::string::npos)
         {
            csv_vector_.push_back(de_name);
            std::cout << csv_vector_.at(csv_vector_.size() - 1) << std::endl;
         }
      }
   }
   else
   {
      std::cerr << "Unable to open directory (CSV.cpp)" << std::endl;
      exit(1);
   }
}
