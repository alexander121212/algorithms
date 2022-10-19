
all: main

main: main.o btree_naive.o
	gcc -o $@ $^

main.o: main.c
	gcc  -c main.c

btree_naive.o: btree_naive.c btree_naive.h
	gcc -c btree_naive.c

clean:
	rm -rf *.o main
