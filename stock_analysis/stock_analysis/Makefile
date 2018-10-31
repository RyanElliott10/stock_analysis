CC = clang++
CFLAGS = -pedantic -Wall -v -Werror -std=c++11
BFLAGS = -lsqlite3
OBJS = analysis.o stock.o data_point.o csv.o
MAIN = analysis

analysis : $(OBJS)
	$(CC) $(OBJS) $(BFLAGS) -o analysis

stock : stock.cpp stock.hpp data_point.hpp
	$(CC) $(CFLAGS) -c stock.cpp

DataPoint : data_point.cpp data_point.hpp
	$(CC) $(CFLAGS) -c data_point.cpp

CSV : csv.cpp csv.hpp
	$(CC) $(CFLAGS) -c csv.cpp
	
clean :
	rm *.o $(MAIN)
