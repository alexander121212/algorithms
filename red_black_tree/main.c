#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

struct red_black_node {
	struct red_black_node *left;
	struct red_black_node *right;
	struct red_black_node *parent;
	int color;
	int data;
};

static void print_rb_tree(struct red_black_node *node, int level)
{
	int i;
	if (node == NULL) {
		return;
	}

	print_rb_tree(node->right, level+1);
	printf("\n");
	for (i = 0; i < level; i++) {
		printf("\t");
	}
	printf("[%d] (%s)", node->data, node->color == RED? "RED" : "BLACK");
	print_rb_tree(node->left, level+1);
}

static void change_color(struct red_black_node **node, int color)
{
	if (*node == NULL)
		return;

	(*node)->color = color;
}

static struct red_black_node * find_node_by_data(struct red_black_node *node, int data)
{
	if (node == NULL) {
		return NULL;
	}

	if (node->data > data) {
		return find_node_by_data(node->left, data);
	} else if (node->data < data) {
		return find_node_by_data(node->right, data);
	} else {
		return node;
	}
}


static void rotate_set_parent(struct red_black_node *old, struct red_black_node *new, struct red_black_node *root, int color)
{
	struct red_black_node *parent = old->parent;
	new->color = old->color;
	old->color = color;

	if (parent) {
		if (parent->left == old) {
			parent->left = new;
		} else {
			parent->right = new;
		}
	} else {
		root = new;
	}
}

static void balance_tree(struct red_black_node *node, struct red_black_node *root)
{
	struct red_black_node *parent = node->parent, *gparent, *tmp;

	while (1) {
		if (parent == NULL) {
			node->color = BLACK;
			break;
		}

		if (parent->color == BLACK) {
			break;
		}

		gparent = parent->parent;
		tmp = gparent->right;
		if (parent != tmp) {
			/* parent == grand parent left  */
			if (tmp && tmp->color == RED) {
				/* Case 1 node's uncle is red
				 * 		G		g
				 *             /  \    --->    /  \
				 *            p    u	      P    U
				 *           /		     /
				 *          n		    n
				 */
				tmp->color = BLACK;
				parent->color = BLACK;
				node = gparent;
				if (node == NULL) {
					printf("Case 1 but node == 0");
					break;
				}
				parent = node->parent;
				node->color = RED;
				continue;
			}

			tmp = parent->right;
			if (node == tmp) {
				/* Case 2 node's uncle is black
				 * and node is parent's right child
				 *		G		G
				 *	       / \	       / \
				 *	      p   U   --->    n   U
				 *             \	     /
				 *		n	    p
				 *
				 */
				tmp = node->left;
				parent->right = tmp;
				node->left = parent;
				if (tmp) {
					tmp->color = BLACK;
				}
				parent->color = RED;
				parent = node;
				tmp = node->right;
			}

			/* Case 3 - node's uncle is black and node is
			 * the parent of left child
			 * 		G		P
			 * 	       / \	       / \
			 * 	      p   U   ----->  n   g
			 * 	     /			   \
			 * 	    n			    U
			 */
			gparent->left = tmp;
			parent->right = gparent;
			if (tmp) {
				tmp->color = BLACK;
			}
			rotate_set_parent(gparent, parent, root, RED);
			break;
		} else {
			/* parent == grand parent right */
			tmp = gparent->left;
			if (tmp && tmp->color == RED) {
				/* Case 1 */
				tmp->color = BLACK;
				parent->color = BLACK;
				node = gparent;
				parent = node->parent;
				node->color = RED;
				continue;
			}

			tmp = parent->left;
			if (node == tmp) {
				/* Case 2 */
				tmp = node->right;
				parent->left = tmp;
				node->right = parent;
				if (tmp) {
					tmp->color = BLACK;
				}

				parent->color = RED;
				parent = node;
				tmp = node->left;
			}

			/* Case 3 */
			gparent->right = tmp;
			parent->left = gparent;
			if (tmp) {
				tmp->color = BLACK;
			}
			rotate_set_parent(gparent, parent, root, RED);
			break;
		}
	}
}

static void insert(struct red_black_node *root, int data)
{
	struct red_black_node **tmp_node;
	struct red_black_node *tmp_parent;

	tmp_node = &root;
	while (*tmp_node) {
		tmp_parent = *tmp_node;
		if ((*tmp_node)->data > data) {
			tmp_node = &(*tmp_node)->left;
		} else if ((*tmp_node)->data < data) {
		        tmp_node = &(*tmp_node)->right;
		} else {
			return;
		}
	}

	*tmp_node = malloc(sizeof(struct red_black_node));
	if (*tmp_node == NULL) {
		return;
	}

	(*tmp_node)->left = (*tmp_node)->right = NULL;
	(*tmp_node)->parent = tmp_parent;
	(*tmp_node)->color = RED;
	(*tmp_node)->data = data;

	balance_tree(*tmp_node, root);
}

static struct red_black_node *find_lagest_node(struct red_black_node *node)
{
	return node->right ? find_lagest_node(node->right) : node;
}

static struct red_black_node *delete(struct red_black_node *node, int data)
{
	if (node == NULL) {
		return NULL;
	}

	if (node->data > data) {
		node->left = delete(node->left, data);
	} else if (node->data > data) {
		node->right = delete(node->right, data);
	} else {
		struct red_black_node *tmp = node;
		if (node->left == NULL && node->right == NULL) {
			node = NULL;
		} else if (node->left != NULL && node->right == NULL) {
			node = node->left;
		} else if (node->right != NULL && node->left == NULL) {
			node = node->right;
		} else {
			tmp = find_lagest_node(node->left);
			node->data = tmp->data;
			node->left = delete(node->left, tmp->data);
		}
		free(tmp);
	}

	return node;
}

static void free_rb_tree(struct red_black_node *node, struct red_black_node *root)
{
	if (node == NULL) {
		return;
	}

	if (node == root) {
		root->left = root->right = NULL;
		return;
	}

	free_rb_tree(node->left, root);
	free_rb_tree(node->right, root);
	free(node);
}

static void test_1(void)
{
	struct red_black_node root =  { NULL, NULL, NULL, BLACK, 50};
	int data[] = {30, 60, 10, 40, 70, 55, 66, 76, 11, 12, 29, 51, 99, 98, 43};
	int i;

	for (i = 0; i < sizeof(data)/ sizeof(data[0]); i++) {
		insert(&root, data[i]);
		printf("\n\n\n");
	}
	print_rb_tree(&root, 0);
	print_rb_tree(&root, 0);

	free_rb_tree(&root, &root);
}

struct test_struct {
	void (*test)(void);
	char *test_name;
};

static struct test_struct tests[] = {
	{ test_1, "test1"}
};

int main(int argc, const char *argv[])
{
	int i;
	for (i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
		tests[i].test();
	}
	return 0;
}
