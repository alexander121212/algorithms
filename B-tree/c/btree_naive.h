#ifndef _BTREE_NAIVE_H_
#define _BTREE_NAIVE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct btree_node_t;

struct btree_node_ops {
	struct btree_node_t *(*ctor)(int degree, bool leaf);
	void (*traverse)(struct btree_node_t *self);
	void (*remove)(struct btree_node_t *self, int k);
	struct btree_node_t *(*search)(struct btree_node_t *self, int k);
	void (*dtor)(struct btree_node_t *self);
};

struct btree_node_t {
	int *keys;
	struct btree_node_t **children;
	int number;
	int degree;
	bool leaf;
	struct btree_node_ops *ops;
};

struct btree_ops {
	struct btree_t *(*ctor)(int degree);
	void (*insert)(struct btree_t *self, int k);
	void (*remove)(struct btree_t *self, int k);
	void (*traverse)(struct btree_t *self);
	struct btree_node_t *(*search)(struct btree_t *self, int k);
	void (*dtor)(struct btree_t *self);
};

struct btree_t {
	struct btree_node_t *root;
	int degree;
	struct btree_ops *ops;
};

struct btree_t *btree_ctor(int degree);

#endif /* _BTREE_NAIVE_H_ */
