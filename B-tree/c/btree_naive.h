#ifndef _BTREE_NAIVE_H_
#define _BTREE_NAIVE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct btree_node_t {
	int *keys;
	struct btree_node_t **children;
	int number;
	int degree;
	bool leaf;
};


struct btree_t {
	struct btree_node_t *root;
	int degree;
};

struct btree_t *btree_ctor(int degree);
void btree_traverse(struct btree_t *self);
void btree_dtor(struct btree_t *self);
void btree_remove(struct btree_t *self, int k);
struct btree_node_t *btree_search(struct btree_t *self, int k);
void btree_insert(struct btree_t *self, int k);

#endif /* _BTREE_NAIVE_H_ */
