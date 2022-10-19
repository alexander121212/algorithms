#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#include "btree_naive.h"


int main(void)
{
	struct btree_t *tree = btree_ctor(3);
	int elems_ins[] = {  1, 3, 7, 10, 11, 13, 14, 15, 18, 16, 19,
			24, 25, 26, 21, 4, 5, 20, 22, 2, 17, 12, 6};
	int elems_rem[] = {6, 13, 7, 4, 2, 16 };
	int elem;

	for (elem = 0; elem < sizeof(elems_ins)/sizeof(elems_ins[0]); elem++) {
		tree->ops->insert(tree, elems_ins[elem]);
	}

	printf("Traversal of tree constructed is\n");
	tree->ops->traverse(tree);
	printf("\n");

	for (elem = 0; elem < sizeof(elems_rem)/sizeof(elems_rem[0]); elem++) {
		tree->ops->remove(tree, elems_rem[elem]);
		printf("Traversal of tree after removing [%d]\n", elems_rem[elem]);
		tree->ops->traverse(tree);
		printf("\n");
	}

	tree->ops->dtor(tree);
	free(tree);
	return 0;
}
