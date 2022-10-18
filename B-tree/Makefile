CC = g++
CPPFLAGS = -std=c++17 -O3 -march=native -ffast-math -funroll-loops

all: main

main: main.o 
	g++ $(CPPFLAGS) -o $@ $^

main.o: main.cpp
	g++ $(CPPFLAGS) -c main.cpp

clean:
	rm -rf *.o main
