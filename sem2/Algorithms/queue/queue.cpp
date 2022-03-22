#include "Queue.h"

Queue::Queue() {
	beg = NULL;
	end = NULL;
	front = NULL;
	back = NULL;
	prev_to_back = NULL;
	prev_to_front = NULL;
	size = 0;
	count = 0; 
	queue_overlaps=false; 

}

void Queue::add_beg(int val) {
	size++;
	if (beg == NULL) { //if list is empty add firs element
		beg = new element;
		beg->value = val;
		beg->pointers_xor = 0; //end,begin and actual are the same element
		end = beg;
	}
	else {

		element* tmp = beg; //save old begin value
		beg = new element; //create new begin 
		beg->value = val;

		beg->pointers_xor = reinterpret_cast<uintptr_t>(tmp) ^ reinterpret_cast<uintptr_t>(end); //xor of end and old begin which is now 2nd element
		tmp->pointers_xor = (tmp->pointers_xor ^ reinterpret_cast<uintptr_t>(end)) ^ reinterpret_cast<uintptr_t>(beg); //update old begin pointers_xor value - next_to_it ^ new_begin
		end->pointers_xor = reinterpret_cast<uintptr_t>(beg) ^ (reinterpret_cast<uintptr_t>(tmp) ^ end->pointers_xor); //update end pointers_xor value - new_begin ^ prev_to_it

		//make sure queue pointers are in right spots

		if (front == tmp) {//tmp = old_beg
			prev_to_front = beg;// prev_to_front = new_beg
		}
		if (back == tmp) { //tmp = old_beg
			prev_to_back = beg;// prev_to_back = new_beg
			
		}
		//if queue starts to loop around the list
		if (size-1 == count && front!=back && front !=end) { // list and queue have the same size, queue dosnt consist of 1 element and queue isnt 1:1 copy of list (front==end && back==beg)
				queue_overlaps = true; //adding new element means queue loops around list  
		}

		if (queue_overlaps)
			count++;


	}

	
}


void Queue::add_end(int val) {
	if (beg == NULL) // if list is empty
		add_beg(val);
	else {

		size++;
		element* tmp = end; //save old end value
		end = new element;
		end->value = val;

		end->pointers_xor = reinterpret_cast<uintptr_t>(tmp) ^ reinterpret_cast<uintptr_t>(beg); //old_end ^ begin
		tmp->pointers_xor = (tmp->pointers_xor ^ reinterpret_cast<uintptr_t>(beg)) ^ reinterpret_cast<uintptr_t>(end); // update old end pointers_xor - prev_to_it ^ new_end
		beg->pointers_xor = reinterpret_cast<uintptr_t>(end) ^ (reinterpret_cast<uintptr_t>(tmp) ^ beg->pointers_xor); //update begin pointers_xot - new_end ^ next_to_begin
		
		 //make sure queue pointers are in right spots

		if (front == tmp) {//tmp = old_end
			prev_to_front = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(end) ^ tmp->pointers_xor); // prev_to_front = prev_to_end
		}
		if (back == tmp)//tmp = old_end
			prev_to_back = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(end) ^ end->pointers_xor); // prev_to_back = prev_to_end
		
		// if queue starts to loop around the list
		if (size-1 == count && front != back && back!=beg) {//list and queue have the same size, queue dosnt consist of 1 element and queue isnt 1:1 copy of list (front==end && back==beg) 
				queue_overlaps = true; //adding new element means queue loops around list  
		}
		
		if (queue_overlaps)
			count++;

	}
}


void Queue::push(int val) {
	count++; 
	if (front == NULL) { //queue is empty 
		if (beg == NULL) { //list is also empty 
			add_beg(val); 
		}
		else {//list is not empty
			end->value = val;  //overwrite value from end
		}
		//this case means there is only one value in queue so both front and back point to the same place

		front = end;
		prev_to_front = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(beg) ^ end->pointers_xor); // prev_to_front = prev_to_end
		back = end;
		prev_to_back = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(beg) ^ end->pointers_xor); // prev_to_back = prev_to_end
	}
	else {
		if (count <= size) {  // list is bigger than queue, so overwrite value previous to back and move back pointer
			element* tmp = prev_to_back;
			prev_to_back =reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(back) ^  prev_to_back->pointers_xor); //move prev_to_back one back
			tmp->value = val; //prev to back (which did not belong to queue but belongs to list) is set to new back and its value is changed 
			back = tmp;
		}
		else {//queue fully covers list, so we add new value, we add before back
			if (back == beg) { //in this case add_beg will handle this case so there is no need to move beg pointer in this function
				add_beg(val);
				//update pointers 
				back = beg; 
				prev_to_back = end;
			}
				
			else {
				size++; // adding new element to list

				element* tmp = back; //save old back value
				back = new element; //create a new one
				back->value = val; 
				
				//update xor values, since new element was inserted 

				prev_to_back->pointers_xor = (prev_to_back->pointers_xor ^ reinterpret_cast<uintptr_t>(tmp)) ^ reinterpret_cast<uintptr_t>(back);// prev to prev_to_back ^ new back
				tmp->pointers_xor = (tmp->pointers_xor ^ reinterpret_cast<uintptr_t>(prev_to_back)) ^ reinterpret_cast<uintptr_t>(back); //tmp is old_back, xor = next to old_back ^ new_back
				back->pointers_xor = reinterpret_cast<uintptr_t>(tmp) ^ reinterpret_cast<uintptr_t>(prev_to_back); //old back ^ prev_to_new_back

			}

		}
	}
}

void Queue::pop() {
	if (count > 0) {
		count--;
		int ret = front->value; //we pop from front 
		if (count == 0) { 
			front = NULL; //queue is empty
			prev_to_front = NULL;
			queue_overlaps = false;
		}
			
		else {//move front one back
			element* tmp = prev_to_front; 
			prev_to_front= reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(front) ^ prev_to_front->pointers_xor); //move prev_to_front one back
			front = tmp;

			if  (front==end) // after any other deletion queue will stop to loop around list
				queue_overlaps = false;
			
		}
		printf("%d\n", ret);


	}
	else //queue is empty
		printf("NULL\n");
}



void Queue::delete_beg() {
	if (beg != NULL) { // no need to delete if list is empty 

		element* next_to_beg = reinterpret_cast<element*>(beg->pointers_xor ^ reinterpret_cast<uintptr_t>(end));
		if (front == beg) { //move front one back
			front = end;
			prev_to_front= reinterpret_cast<element*>(end->pointers_xor ^ reinterpret_cast<uintptr_t>(beg)); //prev_to_front = prev_to_end
		}
		if (back == beg) back = next_to_beg;  //move back to next element 
		//prev_to_back is still end

		if (prev_to_front == beg)  //update pointers so they dont point do deleted value
			prev_to_front = end;

		if (prev_to_back == beg) 
			prev_to_back = end;

		if (beg == back) // after any other deletion queue will stop to loop around list
			if (queue_overlaps)
				queue_overlaps = false;

		if (queue_overlaps)
			count--;

		if (beg == end) { //update end pointer if list has 1 element
			end = NULL;
			delete_element(beg, next_to_beg, end);
			beg = NULL;

		}
		else {
			delete_element(beg, next_to_beg, end);
			beg = next_to_beg;
		}
		
		
	}
}


void Queue::delete_end() {
	if (end != NULL) { // no need to delete if list is empty 
		
		element* prev_to_end = reinterpret_cast<element*>(end->pointers_xor ^ reinterpret_cast<uintptr_t>(beg));
		if (front == end) { //move front one back 
			front = prev_to_end;// prev_to_end will be new end;
			prev_to_front= reinterpret_cast<element*>(prev_to_end->pointers_xor ^ reinterpret_cast<uintptr_t>(end)); //move prev_to_fornt one back
		}
	
		if (back == end) { // move back to next element
			back = beg;
			prev_to_back = prev_to_end; // prev_to_end will be new end;
		}

		if (prev_to_front == end) //update pointers so they dont point do deleted value
			prev_to_front = prev_to_end;

		if (prev_to_back == end)
			prev_to_back = prev_to_end;


		if (end == beg) //update beg pointer if list has 1 element
			beg = NULL;

		if (end == back) // after any other deletion queue will stop to loop around list
			if (queue_overlaps)
				queue_overlaps = false;

		if (queue_overlaps)
			count--;


		delete_element(end, beg, prev_to_end);
		end = prev_to_end;

	}
}





void Queue::delete_element(element* current, element* next_to_current, element* prev_to_current) {
	size--;
	if (next_to_current == NULL && prev_to_current == NULL) { //if prev and next werent passed to function, find them
		next_to_current = get_next(current);
		prev_to_current = get_prev(current);
	}


	if (prev_to_current != NULL)
		prev_to_current->pointers_xor = (prev_to_current->pointers_xor ^ reinterpret_cast<uintptr_t>(current)) ^ reinterpret_cast<uintptr_t>(next_to_current);
	if (next_to_current != NULL)
		next_to_current->pointers_xor = (next_to_current->pointers_xor ^ reinterpret_cast<uintptr_t>(current)) ^ reinterpret_cast<uintptr_t>(prev_to_current);

	delete current;
}




element* Queue::get_next(element* elem) { // function iterates from begin through whole list until it finds element passed as parameter and returns its previous element
	element* tmp = NULL;
	element* current = beg;
	element* prev = end;
	while (prev != elem) {
		tmp = current;
		if (current != NULL)
			current = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(prev) ^ current->pointers_xor); //move current to next to it
		prev = tmp;
	}

	return current;

}


element* Queue::get_prev(element* elem) { // function iterates from end through whole list until it finds element passed as parameter and returns its next element
	element* tmp = NULL;
	element* current = end;
	element* next = beg;
	while (next != elem) {
		tmp = current;
		if (current != NULL)
			current = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(next) ^ current->pointers_xor); //move current to previous to it
		next = tmp;
	}
	return current;

}



void Queue::print_forward() const { //iterate through whole list from begin and print all elements
	element* prev = end;
	element* current = beg;
	if (beg == NULL)
		printf("NULL");
	while (current != NULL) {
		printf("%d ", current->value);
		element* tmp = current;
		current = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(prev) ^ current->pointers_xor); //move current to next to it;
		prev = tmp;
		if (current == beg)
			break;
	}

	printf("\n");
}
void Queue::print_backward() const { //iterate through whole list from end and print all elements
	element* next = beg;
	element* current = end;
	if (end == NULL)
		printf("NULL");
	while (current != NULL) {
		printf("%d ", current->value);
		element* tmp = current;
		current = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(next) ^ current->pointers_xor); //move current to prev to it;
		next = tmp;
		if (current == end)
			break;
	}
	printf("\n");
}

void Queue::print_queue() const {
	element* current = front; //starting at front and movig backwards (forwards in terms of queue, but backwards in terms of list)
	element* prev = prev_to_front;
	if (current == NULL)
		printf("NULL");
	while (current != NULL) {

		printf("%d ", current->value);

		if (current == back) 
			break;

		//move current one back 
		element* tmp = prev;
		prev = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(current) ^ prev->pointers_xor);
		current = tmp;
		
	}
	printf("\n");
}

void Queue::garbage_soft() { //iterate from front forward to back changing every number in between to 0 (theese numbers must not belong to queue, but they belong to list)
	element* current=NULL; 
	element* prev=NULL; //keeping track of prev to current so next_to_current can be accessed
	if (count > 0) {
		if (front != NULL) {
			prev = front;
			current = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(prev_to_front) ^ front->pointers_xor); //current = next_to_front
		}
		while (current != NULL) {
			if (current == back)
				break;
			current->value = 0;
			element* tmp = current; 
			current = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(prev) ^ current->pointers_xor); //current = next_to_current
			prev = tmp;

		}
	}
	else if (size > 0) { //queue is empty so front and back are nullptrs , set whole list to zeros 
		current = beg;
		prev = end;
		while (current != NULL) {
			current->value = 0;
			element* tmp = current;
			current = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(prev) ^ current->pointers_xor); //current = next_to_front
			prev = tmp;
			if (current == beg)
				break;
			
		}
	}

}
void Queue::garbage_hard() { //simillar to garbage_soft() but keeping track of both next and prev for easy deletion
	element* current = NULL;
	element* prev=NULL;
	element* next=NULL;
	if (count > 0) {
		if (front != NULL) { //make sure queue is not empty 
			prev = front;
			current = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(prev_to_front) ^ front->pointers_xor); //set current to next_to_front
			next =  reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(prev) ^ current->pointers_xor); // next to current
		}
		while (current != NULL) {
			if (current == back) //iterate until it doesnt reach back of queue meaning all list elements not belonging to queue are deleted
				break;	
			element* tmp = next; //save next 
			next = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(current) ^ next->pointers_xor); //update next before deletion of current 
			delete_element(current, tmp, prev); //delete current
			current = tmp; //set current to next to it
		
		}
	}
	else if (size > 0) { //queue is empty, so delete whole list
		while (beg != NULL)
			delete_beg();
		}
}

Queue::~Queue() { //delete all the elements
	while (beg != NULL)
		delete_beg();
	end = NULL;
	front = NULL;
	prev_to_front = NULL;
	back = NULL;
	prev_to_back = NULL;
}