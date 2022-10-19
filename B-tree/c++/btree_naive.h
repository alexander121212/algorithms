#ifndef _BTREE_NAIVE_H_
#define _BTREE_NAIVE_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>


class BTreeNode {
public:
	BTreeNode (int degree, bool leaf);

	void insertNotFull(int k);
	void splitChild(int i, BTreeNode *y);
	void traverse() const;

	int findGreaterOrEqual(int k) const;
	void remove(int k);
	void removeFromLeaf(int idx);
	void removeFromNonLeaf(int idx);

	int getPredecessor(int idx);
	int getSuccessor(int idx);

	void fill(int idx);
	void borrowFromPrevious(int idx);
	void borrowFromNext(int idx);

	void mergeChildren(int idx);


	BTreeNode *search(int k);
	friend class BTree;
private:
	int *m_keys;
	BTreeNode **m_children;
	int m_degree;
	int m_number;
	bool m_leaf;
};


class BTree {
public:
	BTree (int degree)
	{
		m_root = NULL;
		m_degree = degree;
	}

	void traverse() const
	{
		if (m_root)
			m_root->traverse();
	}

	BTreeNode *search(int k) const
	{
		return m_root ? m_root->search(k) : NULL;
	}

	void insert(int k);
	void remove(int k);
private:
	BTreeNode *m_root;
	int m_degree;
};


#endif /* _BTREE_NAIVE_H_ */
