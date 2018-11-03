CC = clang++
CFLAGS = -pedantic -Wall -v -Werror -std=c++11
BFLAGS = -lsqlite3
OBJS = main.o stock.o data_point.o csv.o pattern_detector.o
MAIN = analysis

analysis : $(OBJS)
	$(CC) $(OBJS) $(BFLAGS) -o $(MAIN)

stock : stock.cpp stock.hpp data_point.hpp
	$(CC) $(CFLAGS) -c stock.cpp

data_point : data_point.cpp data_point.hpp
	$(CC) $(CFLAGS) -c data_point.cpp

csv : csv.cpp csv.hpp
	$(CC) $(CFLAGS) -c csv.cpp

pattern_detector : pattern_detector.cpp pattern_detector.hpp stock.hpp
	$(CC) $(CFLAGS) -c pattern_detector.cpp
	
clean :
	rm *.o $(MAIN)
