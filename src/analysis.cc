#include <iostream>
#include <string>

#include "stock.h"
#include "data_point.h"
#include "csv.h"

int main(int argc, char *argv[])
{
  std::string input;
  CSV *csv;

  // Prompt user if they want to enter new data into the database
  std::cout << "Enter new data into database? (y/n) ";
  std::cin >> input;
  if (input.find('y') == 0)
  {
    csv = new CSV(std::string("./data/full_history/"));
  }

  if (csv)
  {
    for (int i = 0; i < csv->get_stocks().size(); i++)
    {
      // std::cout << i << " " << std::endl;
      Stock *curr_stock = csv->get_stocks().at(i);
      // std::cout << curr_stock->get_ticker() << " Size: "
      //           << curr_stock->get_data().size() << " "
                // << curr_stock->get_data().at(0)->get_adj_close() << std::endl;

      std::cout << curr_stock->get_ticker() << " Size: "
                << curr_stock->get_data().size() << " " << std::endl;
    }
  }

  return 0;
}