#ifndef NODE_H
#define NODE_H

#include "Entry.h"
#include <iostream>
#include <vector>

typedef std::vector<Entry*> m_entries;

class RTree;

class Node
{
	friend class RTree;

public:
	Node( Node* , size_t );
	~Node();

private:
	size_t level;
	Node* parent;
	m_entries entries;
	
};

Node::Node( Node* parent , size_t level )
{
	this->level = level;
	this->parent = parent;
	std::cout << "Node created..."<< std::endl;
};

#endif // NODE_H