CC = g++
CPPFLAGS = -std=c++17 -O3 -march=native -ffast-math -funroll-loops

all: main

main: main.o naive.o transposition.o vectorization.o
	g++ $(CPPFLAGS) -o main main.o naive.o transposition.o vectorization.o

# The main.o target can be written more simply

main.o: main.cpp
	g++ $(CPPFLAGS) -c main.cpp

naive.o: naive.cpp naive.h
	g++ $(CPPFLAGS) -c naive.cpp

transposition.o: transposition.cpp transposition.h
	g++ $(CPPFLAGS) -c transposition.cpp

vectorization.o: vectorization.cpp vectorization.h
	g++ $(CPPFLAGS) -c vectorization.cpp

clean:
	rm -rf *.o main
