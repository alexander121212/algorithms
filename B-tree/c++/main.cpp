#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "btree_naive.h"

void test_btree_naive(void)
{
	BTree t(3); // A B-Tree with minimum degree 3
	std::vector<int> elems_ins = {  1, 3, 7, 10, 11, 13, 14, 15, 18, 16, 19,
					24, 25, 26, 21, 4, 5, 20, 22, 2, 17, 12, 6};
	std::vector<int> elems_rem = {6, 13, 7, 4, 2, 16 };

	for (auto &elem : elems_ins) {
		t.insert(elem);
	}

	std::cout << "Traversal of tree constructed is\n";
	t.traverse();
	std::cout << std::endl;

	for (auto &elem : elems_rem) {
		t.remove(elem);
		std::cout << "Traversal of tree after removing " << elem << " \n";
		t.traverse();
		std::cout << std::endl;
	}
}

int main(void)
{
	test_btree_naive();
	return 0;
}
