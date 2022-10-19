CFLAGS = -g -ggdb -O0

all: main

main: main.o btree_naive.o
	gcc $(CFLAGS) -o $@ $^

main.o: main.c
	gcc  $(CFLAGS) -c main.c

btree_naive.o: btree_naive.c btree_naive.h
	gcc $(CFLAGS) -c btree_naive.c

clean:
	rm -rf *.o main
