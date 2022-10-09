CC = g++
CPPFLAGS = -std=c++17 -O3 -march=native -ffast-math -funroll-loops

all: main

main: main.o naive.o branchless.o eytzynger.o
	g++ $(CPPFLAGS) -o $@ $^

main.o: main.cpp
	g++ $(CPPFLAGS) -c main.cpp

naive.o: naive.cpp naive.h
	g++ $(CPPFLAGS) -c naive.cpp

branchless.o: branchless.cpp branchless.h
	g++ $(CPPFLAGS) -c branchless.cpp

eytzynger.o: eytzynger.cpp eytzynger.h
	g++ $(CPPFLAGS) -c eytzynger.cpp
clean:
	rm -rf *.o main
