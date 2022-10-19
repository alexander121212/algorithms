#include "btree_naive.h"

static struct btree_node_ops btn_ops;
static struct btree_ops bt_ops;

static void btree_node_traverse(struct btree_node_t *self)
{
	int i = 0;

	while (i < self->number) {
		if (self->leaf == false) {
			self->children[i]->ops->traverse(self->children[i]);
		}
		printf("[%d] ", self->keys[i++]);
	}

	if (self->leaf == false) {
		self->children[i]->ops->traverse(self->children[i]);
	}
}

static struct btree_node_t* btree_node_search(struct btree_node_t *self, int k)
{
	int i = 0;

	while (i < self->number && k > self->keys[i])
		i++;

	if (self->keys[i] == k)
		return self;

	if (self->leaf)
		return NULL;

	return self->children[i]->ops->search(self->children[i], k);
}

static int btree_node_find_greater_or_equal(struct btree_node_t *self, int k)
{
	int idx = 0;

	while (idx < self->number && self->keys[idx] < k)
		idx++;

	return idx;
}

static void remove_from_leaf(struct btree_node_t *self, int idx)
{
	for (int i = idx + 1; i < self->number; i++)
		self->keys[i-1] = self->keys[i];

	self->number--;
}

static int get_predecessor(struct btree_node_t *self, int idx)
{
	struct btree_node_t *cur = self->children[idx+1];

	while (!cur->leaf)
		cur = cur->children[cur->number];

	return cur->keys[cur->number-1];
}


static int get_successor(struct btree_node_t *self, int idx)
{
	struct btree_node_t *cur = self->children[idx+1];

	while (!cur->leaf)
		cur = cur->children[0];

	return cur->keys[0];
}


static void borrow_from_previous(struct btree_node_t *self, int idx)
{
	struct btree_node_t *child = self->children[idx];
	struct btree_node_t *sibling = self->children[idx-1];

	for (int i = child->number - 1; i >= 0; i--)
		child->keys[i+1] = child->keys[i];

	if (!child->leaf)
		for(int i = child->number; i >= 0; i--)
		    child->children[i+1] = child->children[i];

	child->keys[0] = self->keys[idx-1];

	if(!child->leaf)
		child->children[0] = sibling->children[sibling->number];

	self->keys[idx-1] = sibling->keys[sibling->number - 1];

	++child->number;
	--sibling->number;
}


static void borrow_from_next(struct btree_node_t *self, int idx)
{

	struct btree_node_t *child = self->children[idx];
	struct btree_node_t *sibling = self->children[idx+1];

	child->keys[(child->number)] = self->keys[idx];

	if (!(child->leaf))
		child->children[(child->number)+1] = sibling->children[0];

	self->keys[idx] = sibling->keys[0];

	for (int i = 1; i < sibling->number; ++i)
		sibling->keys[i-1] = sibling->keys[i];

	if (!sibling->leaf) {
		for(int i = 1; i <= sibling->number; i++)
			sibling->children[i-1] = sibling->children[i];
	}

	child->number++;
	sibling->number--;
}


static void merge_children(struct btree_node_t *self, int idx)
{
	struct btree_node_t *child = self->children[idx];
	struct btree_node_t *sibling = self->children[idx+1];

	child->keys[self->degree-1] = self->keys[idx];

	for (int i = 0; i < sibling->number; i++)
		child->keys[i + self->degree] = sibling->keys[i];

	if (!child->leaf) {
		for(int i = 0; i <= sibling->number; i++)
		    child->children[i+self->degree] = sibling->children[i];
	}

	for (int i = idx + 1; i < self->number; ++i)
		self->keys[i-1] = self->keys[i];

	for (int i = idx+2; i <= self->number; i++)
		self->children[i-1] = self->children[i];

	child->number += sibling->number+1;
	self->number--;

	free(sibling);
}


static void fill(struct btree_node_t *self, int idx)
{
	if (idx != 0 && self->children[idx-1]->number >= self->degree)
		borrow_from_previous(self, idx);
	else if (idx != self->number && self->children[idx+1]->number >= self->degree)
		borrow_from_next(self, idx);
	else {
		if (idx != self->number)
			merge_children(self, idx);
		else
			merge_children(self, idx-1);
	}
}


static void remove_from_non_leaf(struct btree_node_t *self, int idx)
{
	int k = self->keys[idx];

	if (self->children[idx]->number >= self->degree) {
		int pred = get_predecessor(self, idx);

		self->keys[idx] = pred;
		self->children[idx]->ops->remove(self->children[idx], pred);
	} else if (self->children[idx+1]->number >= self->degree) {
		int succ = get_successor(self, idx);
		self->keys[idx] = succ;
		self->children[idx+1]->ops->remove(self->children[idx+1], succ);
	} else {
		merge_children(self, idx);
		self->children[idx]->ops->remove(self->children[idx], k);
	}
}


static void btree_node_remove(struct btree_node_t *self, int k)
{
	int idx = btree_node_find_greater_or_equal(self, k);

	if (idx < self->number && self->keys[idx] == k) {
		if (self->leaf)
			remove_from_leaf(self, idx);
		else
			remove_from_non_leaf(self, idx);
	} else {
		if (self->leaf) {
			printf("There is no suck key in the tree: %d\n", k);
			return;
		}

		bool flag = ( (idx == self->number) ? true : false);

		if (self->children[idx]->number < self->degree) {
			fill(self, idx);
		}

		if (flag && idx > self->number)
			self->children[idx-1]->ops->remove(self->children[idx-1], k);
		else
			self->children[idx]->ops->remove(self->children[idx], k);
	}
}


static struct btree_node_t *btree_node_ctr(int degree, bool leaf)
{
	struct btree_node_t *node;
	if (degree < 2) {
		degree = 2;
	}

	node = malloc(sizeof(struct btree_node_t));
	if (!node) {
		printf("[%s] cant alloc memory for node\n", __func__);
		return NULL;
	}

	node->degree = degree;
	node->leaf = leaf;

	node->keys = malloc(sizeof(int)*(2*degree - 1));
	if (!node->keys) {
		printf("[%s] can't alloc memory for keys\n", __func__);
		free(node);
		return NULL;
	}

	node->children = malloc(sizeof(*node->children) *(2 * degree));
	if (!node->children) {
		printf("[%s] can't alloc memory for children\n", __func__);
		free(node->keys);
		free(node);
		return NULL;
	}

	node->number = 0;
	node->ops = &btn_ops;

	return node;
}


static void split_child(struct btree_node_t *self, int position, struct btree_node_t *splited_node)
{
	struct btree_node_t *new_node = self->ops->ctor(splited_node->degree, splited_node->leaf);
	if (!new_node) {
		exit(-1);
	}

	new_node->number = self->degree - 1;

	for (int j = 0; j < self->degree-1; j++)
		new_node->keys[j] = splited_node->keys[j + self->degree];

	if (!splited_node->leaf) {
		for (int j = 0; j < self->degree; j++)
			new_node->children[j] = splited_node->children[j+self->degree];
	}

	splited_node->number = self->degree - 1;

	for (int j = self->number; j >= position+1; j--)
		self->children[j+1] = self->children[j];

	self->children[position + 1] = new_node;

	for (int j = self->number - 1; j >= position; j--)
		self->keys[j+1] = self->keys[j];

	self->keys[position] = splited_node->keys[self->degree-1];
	self->number++;
}


static void insert_not_full(struct btree_node_t *self, int k)
{
	int position = self->number - 1;

	if (self->leaf) {
		while (position >= 0 && self->keys[position] > k) {
			self->keys[position+1] = self->keys[position];
			position--;
		}

		self->keys[position+1] = k;
		self->number++;
	} else {
		while (position >= 0 && self->keys[position] > k)
			position--;

		if (self->children[position+1]->number == 2 * self->degree - 1) {
			split_child(self, position+1, self->children[position+1]);

			if (self->keys[position] < k) {
				position++;
			}
		}
		insert_not_full(self->children[position+1], k);
	}
}


static void btree_node_dtor(struct btree_node_t *self)
{
	printf("[%s] \n", __func__);
	return;
}

static struct btree_node_ops btn_ops = {
	.ctor = btree_node_ctr,
	.traverse = btree_node_traverse,
	.search = btree_node_search,
	.remove = btree_node_remove,
	.dtor = btree_node_dtor,
};


static void btree_traverse(struct btree_t *self)
{
	if (self->root)
		self->root->ops->traverse(self->root);
}


static struct btree_node_t *btree_search(struct btree_t *self, int k)
{
	return self->root ? self->root->ops->search(self->root, k) : NULL;
}


static void btree_insert(struct btree_t *self, int k)
{
	if (!self->root) {
		self->root = btree_node_ctr(self->degree, true);
		self->root->keys[0] = k;
		self->root->number = 1;
	} else {
		if (self->root->number == 2 * self->degree - 1) {
			struct btree_node_t *s = self->root->ops->ctor(self->degree, false);

			s->children[0] = self->root;

			split_child(s, 0, self->root);

			int i = 0;
			if (s->keys[0] < k)
				i++;

			insert_not_full(s->children[i], k);

			self->root = s;
		} else {
			insert_not_full(self->root, k);
		}
	}
}


static void btree_remove(struct btree_t *self, int k)
{
	if (!self->root) {
		printf("The tree is empty\n");
		return;
	}

	self->root->ops->remove(self->root, k);

	if (self->root->number == 0) {
		struct btree_node_t *tmp = self->root;
		if (self->root->leaf)
			self->root = NULL;
		else
			self->root = self->root->children[0];

		tmp->ops->dtor(tmp);
		free(tmp);
	}
}

struct btree_t *btree_ctor(int degree)
{
	struct btree_t *tree = malloc(sizeof(struct btree_t));
	if (!tree) {
		printf("[%s] cannot alloc memory for tree\n", __func__);
		return NULL;
	}

	tree->root = NULL;
	tree->degree = degree;
	tree->ops = &bt_ops;
}


static void btree_dtor(struct btree_t *self)
{
	printf("[%s] %p\n", __func__, self);
}

static struct btree_ops bt_ops = {
	.ctor	= btree_ctor,
	.insert = btree_insert,
	.remove = btree_remove,
	.traverse = btree_traverse,
	.search = btree_search,
	.dtor	= btree_dtor,
};
