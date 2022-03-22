#pragma once
#include <cstdint>
#include <iostream>
struct element {
	int value;
	uintptr_t pointers_xor;
};

class List
{
private:
	element* beg;
	element* end;
	element* actual;
	element* prev_to_actual;
	element* get_next(element* elem);
	element* get_prev(element* elem);
public:
	List();
	void add_beg(int val);
	void add_end(int val);
	void delete_actual();
	void delete_beg();
	void delete_end();
	element* get_actual() {
		return actual;
	}
	void next();
	void prev();
	void print_forward() const;
	void print_backward() const;
	void delete_element(element* current, element* next_to_current, element* prev_to_current);
	void del_val(int val);
	void add_act(int val);
	~List();

};