#include <iostream>
#include <string>

#include "stock.h"
#include "data_point.h"
#include "csv.h"

int main(int argc, char *argv[])
{
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

  CSV csv;
  csv.enter_data(std::string("./data/full_history"));

  return 0;
}
