#include <iostream>
#include <string>

#include "stock.h"
#include "data_point.h"
#include "csv.h"

int main(int argc, char *argv[])
{
  std::string input;
  Stock test("AAPL");

  for (long i = 0; i < 1; i++)
  {
    DataPoint *data = new DataPoint("10/11/2018", i, 100, 105, 98, 105);
    test.insert_data_point(data);
  }
  for (int i = 0; i < 1; i++)
  {
    DataPoint *temp = test.get_data().at(i);
    // std::cout << temp->getVolume() << " " << temp->getOpen() << std::endl;
  }

  // Prompt user if they want to enter new data into the database
  std::cout << "Enter new data into database? (y/n) ";
  std::cin >> input;
  if (input.find('y') == 0)
  {
    CSV csv(std::string("./data/full_history"));
  }
  // else
  // {
  //   CSV csv;
  // }
  return 0;
}
