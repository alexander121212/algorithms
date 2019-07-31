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

static void ll_balance_avl_tree(struct avl_tree_node **node)
{
	struct avl_tree_node *tmp_node = *node;
	struct avl_tree_node *tmp_left = (*node)->left;
	struct avl_tree_node *tmp_left_right = (*node)->left->right;

	*node = (*node)->left;
	(*node)->right = tmp_node;
	(*node)->right->left = tmp_left_right;

	(*node)->balance = get_height((*node)->left) - get_height((*node)->right);
	if ((*node)->right) {
		(*node)->right->balance = get_height((*node)->right->left) - get_height((*node)->right->right);
		if ((*node)->right->left) {
			(*node)->right->left->balance = get_height((*node)->right->left->left) - get_height((*node)->right->left->right);
		}
	} 
}

static void rr_balance_avl_tree(struct avl_tree_node **node)
{
	struct avl_tree_node *tmp_node = *node;
	struct avl_tree_node *tmp_right = (*node)->right;
	struct avl_tree_node *tmp_right_left = (*node)->right->left;

	*node = (*node)->right;
	(*node)->left = tmp_node;
	(*node)->left->right = tmp_right_left;

	(*node)->balance = get_height((*node)->left) - get_height((*node)->right);
	if ((*node)->left) {
		(*node)->left->balance = get_height((*node)->left->left) - get_height((*node)->left->right);
		if ((*node)->left->right) {
			(*node)->left->right->balance = get_height((*node)->left->right->left) - get_height((*node)->left->right->right);
		}
	}
}

static void lr_balance_avl_tree(struct avl_tree_node **node)
{
	struct avl_tree_node *tmp_node = *node;
	struct avl_tree_node *tmp_left = (*node)->left;
	struct avl_tree_node *tmp_left_right = (*node)->left->right;
	struct avl_tree_node *tmp_left_right_left = (*node)->left->right->left;
	struct avl_tree_node *tmp_left_right_right = (*node)->left->right->right;

	*node = (*node)->left->right;
	(*node)->left = tmp_left;
	(*node)->right = tmp_node;
	(*node)->left->right = tmp_left_right_left;
	(*node)->right->left = tmp_left_right_right;

	(*node)->balance = get_height((*node)->left) - get_height((*node)->right);
	if ((*node)->left) {
		(*node)->left->balance = get_height((*node)->left->left) - get_height((*node)->left->right);
	}

	if ((*node)->right) {
		(*node)->right->balance = get_height((*node)->right->left) - get_height((*node)->right->right);
	}
}

static void rl_balance_avl_tree(struct avl_tree_node **node)
{
	struct avl_tree_node *tmp_node = *node;
	struct avl_tree_node *tmp_right = (*node)->right;
	struct avl_tree_node *tmp_right_left = (*node)->right->left;
	struct avl_tree_node *tmp_right_left_left = (*node)->right->left->left;
	struct avl_tree_node *tmp_right_left_right = (*node)->right->left->right;

	*node = (*node)->right->left;
	(*node)->left = tmp_node;
	(*node)->right = tmp_right;
	(*node)->left->right = tmp_right_left_left;
	(*node)->right->left = tmp_right_left_right;

	(*node)->balance = get_height((*node)->left) - get_height((*node)->right);
	if ((*node)->left) {
		(*node)->left->balance = get_height((*node)->left->left) - get_height((*node)->left->right);
	}
	if ((*node)->right) {
		(*node)->right->balance = get_height((*node)->right->left) - get_height((*node)->right->right);
	}
}

static void rebalance_avl_tree(struct avl_tree_node **node)
{
	int height = 0;

	height = get_height((*node)->left) - get_height((*node)->right);
	switch(height) {
		case 0:
		case 1:
		case -1:
			(*node)->balance = height;
			break;
		case -2:
			if ((*node)->right && (*node)->right->balance == -1) {
				rr_balance_avl_tree(node);
			} else if ((*node)->right && (*node)->right->balance == 1) {
				rl_balance_avl_tree(node);
			}
			break;
		case 2:
			if ((*node)->left && (*node)->left->balance == 1) {
				ll_balance_avl_tree(node);
			} else if ((*node)->left && (*node)->left->balance == -1) {
				lr_balance_avl_tree(node);
			}
			break;
	}
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

static void insert_avl_node(struct avl_tree_node **node, int data, struct avl_tree_node *root)
{
	if (*node == NULL) {
		struct avl_tree_node *new_node = NULL;
		int i;
		for (i = 0; i < 2; i++) {
			new_node = (struct avl_tree_node*)malloc(sizeof(struct avl_tree_node));
			if (new_node == NULL) {
				return;
			}

			if (search_addr(root, new_node)) {
				continue;
			}

			new_node->left = NULL;
			new_node->right = NULL;
			new_node->data = data;
			new_node->balance = 0;
			*node = new_node;
			return;
		}
	}

	if ((*node)->data > data) {
		insert_avl_node(&(*node)->left, data, root);
	} else if ((*node)->data < data) {
		insert_avl_node(&(*node)->right, data, root);
	} else {
		return;
	}

	rebalance_avl_tree(node);
}

static struct avl_tree_node *find_lagest_node(struct avl_tree_node *node)
{
	return (node->right) ? find_lagest_node(node->right) : node;
}

static void delete_avl_node(struct avl_tree_node **node, int data)
{
	if (*node == NULL) {
		return;
	}

	if ((*node)->data > data) {
		delete_avl_node(&(*node)->left, data);
	} else if ((*node)->data < data) {
		delete_avl_node(&(*node)->right, data);
	} else {
		struct avl_tree_node *tmp = *node;
		if ((*node)->left == NULL && (*node)->right == NULL) {
			*node = NULL;
		} else if ((*node)->left == NULL && (*node)->right) {
			*node = (*node)->right;
		} else if ((*node)->left && (*node)->right == NULL) {
			*node = (*node)->left;
		} else if ((*node)->left && (*node)->right) {
			tmp = find_lagest_node((*node)->left);
			(*node)->data = tmp->data;
			delete_avl_node(&(*node)->left, tmp->data);
			rebalance_avl_tree(node);
		}
		free(tmp);
		return;
	}
	rebalance_avl_tree(node);
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

	insert_avl_node(&root, 19, root);
	insert_avl_node(&root, 9, root);
	insert_avl_node(&root, 18, root);
	insert_avl_node(&root, 63, root);
	insert_avl_node(&root, 27, root);
	insert_avl_node(&root, 99, root);
	insert_avl_node(&root, 81, root);
	insert_avl_node(&root, 108, root);
	insert_avl_node(&root, 109, root);

	print_avl_tree(root, 0);
	printf("\n");

	delete_avl_node(&root, 99);

	print_avl_tree(root, 0);
	printf("\n");

	free_avl_node(root);
}

static void test_2(void)
{
	struct avl_tree_node *root = NULL;

	insert_avl_node(&root, 19, root);
	insert_avl_node(&root, 17, root);
	insert_avl_node(&root, 29, root);
	insert_avl_node(&root, 18, root);
	insert_avl_node(&root, 14, root);
	insert_avl_node(&root, 15, root);
	insert_avl_node(&root, 13, root);
	insert_avl_node(&root, 12, root);

	print_avl_tree(root, 0);
	printf("\n");
	delete_avl_node(&root, 14);

	print_avl_tree(root, 0);
	printf("\n");
	free_avl_node(root);
}

static void test_3(void)
{
	struct avl_tree_node *root = NULL;

	insert_avl_node(&root, 17, root);
	insert_avl_node(&root, 10, root);
	insert_avl_node(&root, 40, root);
	insert_avl_node(&root, 9, root);
	insert_avl_node(&root, 15, root);
	insert_avl_node(&root, 38, root);
	insert_avl_node(&root, 41, root);
	insert_avl_node(&root, 12, root);
	insert_avl_node(&root, 16, root);
	insert_avl_node(&root, 11, root);

	print_avl_tree(root, 0);
	printf("\n");
	delete_avl_node(&root, 17);

	print_avl_tree(root, 0);
	printf("\n");
	free_avl_node(root);
}

static void test_4(void)
{
	struct avl_tree_node *root = NULL;

	insert_avl_node(&root, 1, root);
	insert_avl_node(&root, 2, root);
	insert_avl_node(&root, 3, root);
	insert_avl_node(&root, 4, root);
	insert_avl_node(&root, 5, root);
	insert_avl_node(&root, 6, root);
	insert_avl_node(&root, 7, root);

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
