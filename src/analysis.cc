/**
 * MOVE THIS TO THE README.MD EVENTUALLY
 * 
 * Coding standard
 * 
 * 2 spaces
 * files are to be named_like_this.cc (NOT .cpp)
 * StrongCamelCase for classes, structs, enums, etc. (all types)
 * underscore_vars for vars and public functions
 * _underscore_funcs for private functions
 * struct_data_members follow normal variable naming conventions
 * var_name_ for class members
 */

#include <iostream>
#include <string>

#include "stock.h"
#include "data_point.h"
#include "csv.h"

int main(int argc, char *argv[])
{
  Stock test("AAPL");

  for (int i = 0; i < 1; i++)
  {
    DataPoint *data = new DataPoint(i, 100, 105, 98, 105);
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
