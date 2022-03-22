#pragma once
#include <iostream>
#include <cstdio>





class Node
{
private: 
	int T;
	bool is_leaf;
	int keys_count;
	int* keys;
	Node** sons;

	friend class Tree; //tree has to have acces to all internal variables of node, so it is better solution to declarate it as friend class than keeping all variables as public

public:

	Node(int T, bool leaf);
	~Node();

	void split_son(int position);
	void insert_not_full(int value);

	bool is_full() const {
		return keys_count >= (T * 2 - 1);
	}
	void print_down(FILE* stream, bool values);
	void check_node(int pos);
	void remove(int x);
	void remove_from_leaf(int pos);
	void remove_from_node(int pos);
	void merge(int pos);
	int get_succ(int pos) const;
	int get_pred(int pos) const;
	void borrow_left(int pos);
	void borrow_right(int pos);
	Node& operator= (const Node& other);
	Node(const Node& other);
};

