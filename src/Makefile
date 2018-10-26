CC = clang++
FLAGS = -pedantic -Wall
OBJS = analysis.o stock.o data_point.o csv.o
MAIN = analysis

analysis : $(OBJS)
	$(CC) $(OBJS) -o analysis

stock : stock.cpp stock.h data_point.h
	$(CC) $(CFLAGS) -c stock.cpp

DataPoint : data_point.cpp data_point.h
	$(CC) $(CFLAGS) -c data_point.cpp

CSV : csv.cpp csv.h
	$(CC) $(CFLAGS) -c csv.cpp
	
clean :
	rm *.o $(MAIN)
