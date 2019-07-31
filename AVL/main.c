#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct avl_tree_node {
	struct avl_tree_node *left;
	struct avl_tree_node *right;
	int data;
	int balance;
};

static void print_avl_tree(struct avl_tree_node *node, int level)
{
	int i;
	if (node == NULL) {
		return;
	}

	print_avl_tree(node->right, level+1);
	printf("\n");
	for (i = 0; i < level; i++) {
		printf("\t");
	}
	printf("[%d] (%d)", node->data, node->balance);
	print_avl_tree(node->left, level+1);
}

static int get_height(struct avl_tree_node *node)
{
	int leftheight, rightheight;
	if (node == NULL) {
		return 0;
	}

	leftheight = get_height(node->left);
	rightheight = get_height(node->right);

	return (leftheight > rightheight) ? (leftheight + 1) : (rightheight + 1);
}

#define LL_ROTATION(n)	 		\
	do {				\
		struct avl_tree_node *a;\
		struct avl_tree_node *b;\
		(a) = (node);		\
		(b) = (node)->left;	\
		(a)->left = (b)->right;	\
		(a)->balance = 0;	\
		(b)->right = (a);	\
		(b)->balance = 0;	\
		(node) = (b);		\
	} while (0);

#define LR_ROTATION(n)				\
	do {					\
		struct avl_tree_node *a;	\
		struct avl_tree_node *b;	\
		struct avl_tree_node *c;	\
		(a) = (node);			\
		if ((a)->right) {		\
			(a)->balance = -1;	\
		} else {			\
			(a)->balance = 0;	\
		}				\
		(b) = (node)->left;		\
		(b)->balance = 0;		\
		(c) = (node)->left->right;	\
		(a)->left = (c)->right;		\
		(b)->right = (c)->left;		\
		(c)->left = (b);		\
		(c)->right = (a);		\
		(c)->balance = 0;		\
		(node) = (c);			\
	} while (0);

static struct avl_tree_node *left_rebalance_avl_tree(struct avl_tree_node *node, int *new)
{
	switch(node->balance) {
		case 1:
			if (node->left->balance == 1) {
				LL_ROTATION(node);
			} else {
				LR_ROTATION(node);
			}
			*new = 0;
			break;
		case 0:
			node->balance = 1;
			break;
		case -1:
			node->balance = 0;
			*new = 0;
			break;
	}

	return node;
}

#define RR_ROTATION(node)		\
	do {				\
		struct avl_tree_node *a;\
		struct avl_tree_node *b;\
		(a) = (node);		\
		(b) = (node)->right;	\
		(a)->right = (b)->left;	\
		(a)->balance = 0;	\
		(b)->left = (a);	\
		(b)->balance = 0;	\
		(node) = (b);		\
	} while (0);

#define RL_ROTATION(node)			\
	do {					\
		struct avl_tree_node *a;	\
		struct avl_tree_node *b;	\
		struct avl_tree_node *c;	\
		(a) = (node);			\
		if ((a)->balance == -1) {	\
			(a)->balance = 0;	\
		} else {			\
			(a)->balance = 1;	\
		}				\
		(b) = (node)->right;		\
		if ((b)->right) {		\
			(b)->balance = -1;	\
		} else {			\
			(b)->balance = 0;	\
		}				\
		(c) = (node)->right->left;	\
		(a)->right = (c)->left;		\
		(b)->left = (c)->right;		\
		(c)->left = (a);		\
		(c)->right = (b);		\
		(c)->balance = 0;		\
		(node) = (c);			\
	} while (0);

static struct avl_tree_node *right_rebalance_avl_tree(struct avl_tree_node *node, int *new)
{
	switch(node->balance) {
		case -1:
			if (node->right->balance == -1) {
				RR_ROTATION(node);
			} else {
				RL_ROTATION(node);
			}
			*new = 0;
			break;
		case 0:
			node->balance = -1;
			break;
		case 1:
			node->balance = 0;
			*new = 0;
			break;
	}

	return node;
}

static int search_addr(struct avl_tree_node *node1, struct avl_tree_node *node2)
{
	if (node1 == NULL) {
		return 0;
	}

	if (node1 == node2) {
		return 1;
	}

	if (search_addr(node1->left, node2)) {
		return 1;
	}

	if (search_addr(node1->right, node2)) {
		return 1;
	}
	return 0;
}

static struct avl_tree_node *insert_avl_node(struct avl_tree_node *node, int data, int *new, struct avl_tree_node *root)
{
	if (node == NULL) {
		struct avl_tree_node *new_node = NULL;
		int i;
		for (i = 0; i < 2; i++) {
			node = (struct avl_tree_node*)malloc(sizeof(struct avl_tree_node));
			if (node == NULL) {
				continue;
			}

			if (search_addr(root, node)) {
				continue;
			}

			node->left = NULL;
			node->right = NULL;
			node->data = data;
			node->balance = 0;
			*new = 1;
		}
		return node;
	}

	if (node->data > data) {
		node->left = insert_avl_node(node->left, data, new, root);
		if (*new) {
			node = left_rebalance_avl_tree(node, new);
		}
	} else if (node->data < data) {
		node->right = insert_avl_node(node->right, data, new, root);
		if (*new) {
			node = right_rebalance_avl_tree(node, new);
		}
	}

	return node;
}

static struct avl_tree_node *find_lagest_node(struct avl_tree_node *node)
{
	return (node->right) ? find_lagest_node(node->right) : node;
}

static void free_avl_node(struct avl_tree_node *node)
{
	if (node == NULL) {
		return;
	}

	free_avl_node(node->left);
	free_avl_node(node->right);
	free(node);
}

static void test_1(void)
{
	struct avl_tree_node *root = NULL;
	int nodes_data[] = { 19, 9, 18, 63, 27, 99, 81, 108, 109};
	int new = 0, i;

	for (i = 0; i < sizeof(nodes_data)/sizeof(nodes_data[0]); i++) {
		root = insert_avl_node(root, nodes_data[i], &new, root);
	}

	print_avl_tree(root, 0);
	printf("\n");
	free_avl_node(root);
}

static void test_2(void)
{
	struct avl_tree_node *root = NULL;
	int nodes_data[] = { 19, 17, 29, 18, 14, 15, 13, 12};
	int new = 0, i;

	for (i = 0; i < sizeof(nodes_data)/sizeof(nodes_data[0]); i++) {
		root = insert_avl_node(root, nodes_data[i], &new, root);
	}

	print_avl_tree(root, 0);
	printf("\n");
	free_avl_node(root);
}

static void test_3(void)
{
	struct avl_tree_node *root = NULL;
	int nodes_data[] = { 17, 10, 40, 9, 15, 38, 41, 12, 16, 11};
	int new = 0, i;

	for (i = 0; i < sizeof(nodes_data)/sizeof(nodes_data[0]); i++) {
		root = insert_avl_node(root, nodes_data[i], &new, root);
	}
	print_avl_tree(root, 0);
	printf("\n");
	free_avl_node(root);
}

static void test_4(void)
{
	struct avl_tree_node *root = NULL;
	int nodes_data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	int new = 0, i;

	for (i = 0; i < sizeof(nodes_data)/sizeof(nodes_data[0]); i++) {
		root = insert_avl_node(root, nodes_data[i], &new, root);
	}
	print_avl_tree(root, 0);
	printf("\n");
	free_avl_node(root);
}

struct test_struct {
	char *name;
	void (*test_func)(void);
};

static struct test_struct tests[] = {
	{"test_1", test_1},
	{"test_2", test_2},
	{"test_3", test_3},
	{"test_4", test_4}
};

int main(int argc, const char *argv[])
{
	int i;

	for (i = 0; i < sizeof(tests)/sizeof(struct test_struct); i++) {
		printf("Test name %s", tests[i].name);
		tests[i].test_func();
	}

	return 0;
}
