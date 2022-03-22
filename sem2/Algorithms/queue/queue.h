#pragma once
#include <cstdint>
#include <iostream>
struct element {
	int value;
	uintptr_t pointers_xor;
};

class Queue
{
private:
	int size;
	int count;
	element* beg;
	element* front;
	element* prev_to_front;
	element* back;
	element* prev_to_back;
	element* end;
	element* get_next(element* elem );
	element* get_prev(element* elem);
	bool queue_overlaps; //bool indicating that add_beg() and add_end() operations will add also to queue, 
	//	example of such a situation would be list in state:	 3 4 5 6 7 8 and queue: 4 3 8 7, where add_beg() would add an element to queue
    //														   |	 |
	//														  front  back
public:
	Queue();
	int get_size()  const {
		return size;
	}
	int get_count() const {
		return count;
	}

	void push(int val);
	void pop();
	void garbage_soft();
	void garbage_hard();
	void add_beg(int val);
	void add_end(int val);
	void delete_beg();
	void delete_end();
	void print_queue() const;
	void print_forward() const;
	void print_backward() const;
	void delete_element(element* current, element* next_to_current, element* prev_to_current);
	~Queue();

};