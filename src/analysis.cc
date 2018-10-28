#include <string>
#include <iostream>

#include "csv.h"
#include "stock.h"
#include "data_point.h"

int main(int argc, char *argv[])
{
  std::string input;
  CSV *csv;

  // Prompt user if they want to enter new data into the database
  std::cout << "Enter new data into database? (y/n) ";
  std::cin >> input;
  if (input.find('y') == 0)
  {
    csv = new CSV(std::string("./data/test_history/"));
    csv->update_db();
  }

  return 0;
}