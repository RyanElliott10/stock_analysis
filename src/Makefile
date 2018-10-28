CC = clang++
CFLAGS = -pedantic -Wall -v -Werror -std=c++11
BFLAGS = -lsqlite3
OBJS = analysis.o stock.o data_point.o csv.o
MAIN = analysis

analysis : $(OBJS)
	$(CC) $(OBJS) $(BFLAGS) -o analysis

stock : stock.cc stock.h data_point.h
	$(CC) $(CFLAGS) -c stock.cc

DataPoint : data_point.cc data_point.h
	$(CC) $(CFLAGS) -c data_point.cc

CSV : csv.cc csv.h
	$(CC) $(CFLAGS) -c csv.cc
	
clean :
	rm *.o $(MAIN)
