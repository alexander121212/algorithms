CC = g++
CPPFLAGS = -std=c++17 -O3 -march=native -ffast-math -funroll-loops

all: main

main: main.o btree_naive.o
	g++ $(CPPFLAGS) -o $@ $^

main.o: main.cpp
	g++ $(CPPFLAGS) -c main.cpp

btree_naive.o: btree_naive.cpp btree_naive.h
	g++ $(CPPFLAGS) -c btree_naive.cpp

clean:
	rm -rf *.o main
