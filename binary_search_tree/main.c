#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tree_node {
	int data;
	struct tree_node *left;
	struct tree_node *right;
};

static int add_new_node(struct tree_node **node, int data)
{
	struct tree_node *new_node;
	new_node = malloc(sizeof(struct tree_node));
	if (new_node == NULL) {
		return -1;
	}

	new_node->data = data;

	if ((*node)->data > data) {
		(*node)->left = new_node;
	} else if ((*node)->data < data) {
		(*node)->right = new_node;
	}

	return 0;
}

static int add_node(struct tree_node **node, int data)
{
	struct tree_node *new_node;
	struct tree_node *tmp;

	if (*node == NULL) {
		*node = malloc(sizeof(struct tree_node));
		if (*node == NULL) {
			return -1;
		}

		(*node)->data = data;
		return 0;
	}

	tmp = *node;
	while (tmp->left != NULL || tmp->right != NULL) {
		if (data < tmp->data) {
			if (tmp->left) {
				tmp = tmp->left;
				continue;
			}
			break;
		} else if (data > tmp->data) {
			if (tmp->right) {
				tmp = tmp->right;
				continue;
			}
			break;
		} else {
			return -1;
		}
	}

	return add_new_node(&tmp, data);
}

static void insert_node(struct tree_node **node, int val)
{
	if (*node == NULL) {
		*node = malloc(sizeof(struct tree_node));
		if (*node == NULL) {
			return;
		}

		(*node)->left = (*node)->right = NULL;
		(*node)->data = val;
	} else {
		if (val < (*node)->data) {
			insert_node(&(*node)->left, val);
		} else  if (val > (*node)->data) {
			insert_node(&(*node)->right, val);
		}
	}

	return;
}

static struct tree_node* find_node(struct tree_node* node, int data)
{
	struct tree_node *result = NULL;

	if (node == NULL) {
		return NULL;
	}

	if (node->data == data) {
		return node;
	}

	result = find_node(node->left, data);
	if (result != NULL) {
		return result;
	}

	result = find_node(node->right, data);
	if (result != NULL) {
		return result;
	}

	return NULL;
}

static struct tree_node *find_lagest_node(struct tree_node *node)
{
	return (node->right) ? find_lagest_node(node->right) : node;
}

static void delete_node_by_val(struct tree_node **node, int data)
{
	if (*node == NULL) {
		return;
	}

	if ((*node)->data > data) {
		delete_node_by_val(&(*node)->left, data);
	} else if ((*node)->data < data) {
		delete_node_by_val(&(*node)->right, data);
	} else {  /* (node->data == data) */
		struct tree_node *tmp = *node;
		if ((*node)->left && (*node)->right) {
			tmp = find_lagest_node((*node)->left);
			(*node)->data = tmp->data;
			delete_node_by_val(&(*node)->left, tmp->data);
		} else if ((*node)->left == NULL) {
			*node = (*node)->right;
		} else if ((*node)->right == NULL) {
			*node = (*node)->right;
		} else { /* (*node)->left == NULL && (*node)->right == NULL*/
			*node = NULL;
		}
		free(tmp);
	}
}

static void free_tree(struct tree_node *node)
{
	if (node == NULL) {
		return;
	}

	free_tree(node->left);
	free_tree(node->right);

	free(node);
}

static void preorder_deep(struct tree_node *node)
{
	if (node == NULL) {
		return;
	}

	printf("%d ", node->data);
	preorder_deep(node->left);
	preorder_deep(node->right);
}

static void inorder_deep(struct tree_node *node)
{
	if (node == NULL) {
		return;
	}

	inorder_deep(node->left);
	printf("%d ", node->data);
	inorder_deep(node->right);
}

static void postorder_deep(struct tree_node *node)
{
	if (node == NULL) {
		return;
	}

	postorder_deep(node->left);
	postorder_deep(node->right);
	printf("%d ", node->data);
}

struct bfs_queue {
	struct tree_node *node;
	struct bfs_queue *next;
};

static int push_queue(struct bfs_queue **queue, struct tree_node *node)
{
	struct bfs_queue *new_queue;
	struct bfs_queue *tmp_queue;
	if (node == NULL) {
		return -1;
	}

	new_queue = malloc(sizeof(struct bfs_queue));
	if (new_queue == NULL) {
		return -1;
	}

	new_queue->node = node;
	
	if (*queue == NULL) {
		*queue = new_queue;
	} else {
		tmp_queue = *queue;
		for (; (*queue)->next != NULL; (*queue) = (*queue)->next);

		(*queue)->next = new_queue;
		(*queue) = tmp_queue;
	}
	return 0;
}

static struct bfs_queue *pop_queue(struct bfs_queue **queue)
{
	struct bfs_queue *popped = malloc(sizeof(struct bfs_queue));
	if (popped == NULL) {
		return NULL;
	}

	memcpy(popped, *queue, sizeof(struct bfs_queue));
	popped->next = NULL;

	*queue = (*queue)->next;

	return popped;
}

#if 1
static void bfs(struct tree_node *node)
{
	struct bfs_queue *queue;
	struct bfs_queue *tmp_queue;
	if (node == NULL) {
		return;
	}

	queue = malloc(sizeof(struct bfs_queue));
	if (queue == NULL) {
		return;
	}

	queue->next = NULL;
	queue->node = node;

	while (queue) {
		tmp_queue = pop_queue(&queue);
		if (tmp_queue->node && tmp_queue->node->left) {
			push_queue(&queue, tmp_queue->node->left);
		}

		if (tmp_queue->node && tmp_queue->node->right) {
			push_queue(&queue, tmp_queue->node->right);
		}

		printf("%d\n", tmp_queue->node->data);
	}

	return;
}
#else
static void bfs(struct tree_node *node)
{
}
#endif

int main(int argc, const char *argv[])
{
	struct tree_node *root = NULL;

	insert_node(&root, 8);
	insert_node(&root, 4);
	insert_node(&root, 12);
	insert_node(&root, 2);
	insert_node(&root, 5);
	insert_node(&root, 9);
	insert_node(&root, 14);
	insert_node(&root, 3);
	insert_node(&root, 7);
	insert_node(&root, 10);
	printf("\nPre-order deep search\n");
	preorder_deep(root);
	printf("\nInorder deep search\n");
	inorder_deep(root);
	printf("\nPost-order deep search\n");
	postorder_deep(root);
	printf("\nBFS\n");
	bfs(root);

	delete_node_by_val(&root, 4);
	printf("\nPre-order deep search\n");
	preorder_deep(root);
	free_tree(root);
	return 0;
}
