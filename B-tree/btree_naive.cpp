#include "btree_naive.h"


BTreeNode::BTreeNode(int degree, bool leaf)
{
	if (degree < 2) {
		degree = 2;
	}

	m_degree = degree;
	m_leaf = leaf;

	m_keys = new int[2*degree - 1];
	m_children = new BTreeNode *[2 * m_degree];
	m_number = 0;
}


void BTreeNode::traverse() const
{
	int i = 0;

	while (i < m_number) {
		if (m_leaf == false) {
			m_children[i]->traverse();
		}
		std::cout << " " << m_keys[i++];
	}

	if (m_leaf == false) {
		m_children[i]->traverse();
	}
}


BTreeNode* BTreeNode::search(int k)
{
	int i = 0;

	while (i < m_number && k > m_keys[i])
		i++;

	if (m_keys[i] == k)
		return this;

	if (m_leaf)
		return NULL;

	return m_children[i]->search(k);
}


int BTreeNode::findGreaterOrEqual(int k) const
{
	int idx = 0;

	while (idx < m_number && m_keys[idx] < k)
		idx++;

	return idx;
}


void BTreeNode::remove(int k)
{
	int idx = findGreaterOrEqual(k);

	if (idx < m_number && m_keys[idx] == k) {
		if (m_leaf)
			removeFromLeaf(idx);
		else
			removeFromNonLeaf(idx);
	} else {
		if (m_leaf) {
			std::cout << "There is no suck key in the tree: " << k << std::endl;
			return;
		}

		bool flag = ( (idx == m_number) ? true : false);

		if (m_children[idx]->m_number < m_degree) {
			fill(idx);
		}

		if (flag && idx > m_number)
			m_children[idx-1]->remove(k);
		else
			m_children[idx]->remove(k);
	}
}


void BTreeNode::removeFromLeaf(int idx)
{
	for (int i = idx + 1; i < m_number; i++)
		m_keys[i-1] = m_keys[i];

	m_number--;
}


void BTreeNode::removeFromNonLeaf(int idx)
{
	int k = m_keys[idx];

	if (m_children[idx]->m_number >= m_degree) {
		int pred = getPredecessor(idx);
		m_keys[idx] = pred;
		m_children[idx]->remove(pred);
	} else if (m_children[idx+1]->m_number >= m_degree) {
		int succ = getSuccessor(idx);
		m_keys[idx] = succ;
		m_children[idx+1]->remove(succ);
	} else {
		mergeChildren(idx);
		m_children[idx]->remove(k);
	}
}


int BTreeNode::getPredecessor(int idx)
{
	BTreeNode *cur = m_children[idx+1];

	while (!cur->m_leaf)
		cur = cur->m_children[cur->m_number];

	return cur->m_keys[cur->m_number-1];
}


int BTreeNode::getSuccessor(int idx)
{
	BTreeNode *cur = m_children[idx+1];

	while (!cur->m_leaf)
		cur = cur->m_children[0];

	return cur->m_keys[0];
}


void BTreeNode::fill(int idx)
{
	if (idx != 0 && m_children[idx-1]->m_number >= m_degree)
		borrowFromPrevious(idx);
	else if (idx != m_number && m_children[idx+1]->m_number >= m_degree)
		borrowFromNext(idx);
	else {
		if (idx != m_number)
			mergeChildren(idx);
		else
			mergeChildren(idx-1);
	}
}


void BTreeNode::borrowFromPrevious(int idx)
{

	BTreeNode *child = m_children[idx];
	BTreeNode *sibling = m_children[idx-1];

	for (int i = child->m_number - 1; i >= 0; i--)
		child->m_keys[i+1] = child->m_keys[i];

	if (!child->m_leaf)
		for(int i = child->m_number; i >= 0; i--)
		    child->m_children[i+1] = child->m_children[i];

	child->m_keys[0] = m_keys[idx-1];

	if(!child->m_leaf)
		child->m_children[0] = sibling->m_children[sibling->m_number];

	m_keys[idx-1] = sibling->m_keys[sibling->m_number - 1];

	child->m_number += 1;
	sibling->m_number -= 1;
}


void BTreeNode::borrowFromNext(int idx)
{

	BTreeNode *child = m_children[idx];
	BTreeNode *sibling = m_children[idx+1];

	child->m_keys[(child->m_number)] = m_keys[idx];

	if (!(child->m_leaf))
		child->m_children[(child->m_number)+1] = sibling->m_children[0];

	m_keys[idx] = sibling->m_keys[0];

	for (int i = 1; i < sibling->m_number; ++i)
		sibling->m_keys[i-1] = sibling->m_keys[i];

	if (!sibling->m_leaf) {
		for(int i = 1; i <= sibling->m_number; i++)
			sibling->m_children[i-1] = sibling->m_children[i];
	}

	child->m_number++;
	sibling->m_number--;
}


void BTreeNode::mergeChildren(int idx)
{
	BTreeNode *child = m_children[idx];
	BTreeNode *sibling = m_children[idx+1];

	child->m_keys[m_degree-1] = m_keys[idx];

	for (int i = 0; i < sibling->m_number; i++)
		child->m_keys[i + m_degree] = sibling->m_keys[i];

	if (!child->m_leaf) {
		for(int i = 0; i <= sibling->m_number; i++)
		    child->m_children[i+m_degree] = sibling->m_children[i];
	}

	for (int i = idx + 1; i < m_number; ++i)
		m_keys[i-1] = m_keys[i];

	for (int i = idx+2; i <= m_number; i++)
		m_children[i-1] = m_children[i];

	child->m_number += sibling->m_number+1;
	m_number--;

	delete(sibling);
}


void BTreeNode::insertNotFull(int k)
{
	int position = m_number - 1;

	if (m_leaf) {
		while (position >= 0 && m_keys[position] > k) {
			m_keys[position+1] = m_keys[position];
			position--;
		}

		m_keys[position+1] = k;
		m_number++;
	} else {
		while (position >= 0 && m_keys[position] > k)
			position--;

		if (m_children[position+1]->m_number == 2 * m_degree - 1) {
			splitChild(position+1, m_children[position+1]);

			if (m_keys[position] < k) {
				position++;
			}
		}
		m_children[position+1]->insertNotFull(k);
	}
}


void BTreeNode::splitChild(int position, BTreeNode *splitedNode)
{
	BTreeNode *newNode = new BTreeNode(splitedNode->m_degree, splitedNode->m_leaf);

	newNode->m_number = m_degree - 1;

	for (int j = 0; j < m_degree-1; j++)
		newNode->m_keys[j] = splitedNode->m_keys[j+m_degree];

	if (!splitedNode->m_leaf) {
		for (int j = 0; j < m_degree; j++)
			newNode->m_children[j] = splitedNode->m_children[j+m_degree];
	}

	splitedNode->m_number = m_degree - 1;

	for (int j = m_number; j >= position+1; j--)
		m_children[j+1] = m_children[j];

	m_children[position + 1] = newNode;

	for (int j = m_number - 1; j >= position; j--)
		m_keys[j+1] = m_keys[j];

	m_keys[position] = splitedNode->m_keys[m_degree-1];
	m_number++;
}


void BTree::insert(int k)
{
	if (!m_root) {
		m_root = new BTreeNode(m_degree, true);
		m_root->m_keys[0] = k;
		m_root->m_number = 1;
	} else {
		if (m_root->m_number == 2 * m_degree - 1) {
			BTreeNode *s = new BTreeNode(m_degree, false);

			s->m_children[0] = m_root;

			s->splitChild(0, m_root);

			int i = 0;
			if (s->m_keys[0] < k)
				i++;

			s->m_children[i]->insertNotFull(k);

			m_root = s;
		} else {
			m_root->insertNotFull(k);
		}
	}
}


void BTree::remove(int k)
{
	if (!m_root) {
		std::cout << "The tree is empty\n";
		return;
	}

	m_root->remove(k);

	if (m_root->m_number == 0) {
		BTreeNode *tmp = m_root;
		if (m_root->m_leaf)
			m_root = NULL;
		else
			m_root = m_root->m_children[0];

		delete tmp;
	}
}
