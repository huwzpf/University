#pragma once
#include <iostream>
using namespace std;



template <class Type>
class List
{

public:
	typedef struct element {
		element* next;
		element* prev;
		Type content;
	};

	List();

	Type get_end() {
		return end->content;
	}
	Type get_beg() {
		return beg->content;
	}
	void insert_after(element* arg, Type cont);
	void delete_last();
	void delete_first();
	void insert_first(Type arg);
	void insert_last(Type arg);
	void print();
	element* search(Type arg) {
		element* temp = beg;
		while (temp != nullptr) {
			if (temp->content == arg) {
				return temp;
			}
			temp = temp->next;
		}
		return nullptr;
	}
	bool is_empty() {
		if (beg == nullptr) {
			return true;
		}
		else return false;
	}

private:
	Type content;
	element* end;
	element* beg;


};

