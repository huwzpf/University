#pragma once
#include "Node.h"
#include "List.h"

 struct construction_return { //structure needed for construct_branch() function used in loading tree 
	Node* returned_node;
	element* current;
};

class Tree
{
private:
	int T;
	Node* root;


public:
	friend class Node;
	Tree(int t) : T(t) { root = nullptr; }
	~Tree();
	void delete_branch(Node* n);

	void insert(int val);
	void print(FILE* stream, bool values); //print takes file pointer so it can print both to stdout and file
	void construct_tree();
	construction_return construct_branch(List* l, element* begin, element* end);

	bool search_value(int x) const { //search() returns node pointer, this function returns information if value x is present in tree 
		if (search(x) == nullptr)
			return false;
		else
			return true;
	}

	Node* search(int x) const;
	int count_search(int x) const;

	void calculate_cache(FILE* in) const;
	void remove(int x);
};

