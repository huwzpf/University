#include "List.h"

List::List(){
	beg = NULL;
	end = NULL;
	actual = NULL;
	prev_to_actual = NULL;
}

void List::add_beg(int val) {
	if (beg == NULL) { //if list is empty add firs element
		beg = new element; 
		beg->value = val;
		beg->pointers_xor = 0; //end,begin and actual are the same element
		end = beg; 
		actual = beg;
		prev_to_actual = end;
	}
	else { 
		element* tmp = beg; //save old begin value
		beg = new element; //create new begin 
		beg->value = val;

		beg->pointers_xor = reinterpret_cast<uintptr_t>(tmp) ^ reinterpret_cast<uintptr_t>(end); //xor of end and old begin which is now 2nd element
		tmp->pointers_xor = (tmp->pointers_xor ^ reinterpret_cast<uintptr_t>(end)) ^ reinterpret_cast<uintptr_t>(beg); //update old begin pointers_xor value - next_to_it ^ new_begin
		end->pointers_xor = reinterpret_cast<uintptr_t>(beg) ^ (reinterpret_cast<uintptr_t>(tmp) ^ end->pointers_xor); //update end pointers_xor value - new_begin ^ prev_to_it
		if (actual==tmp) { //if actual was 1st element assign prev_to_actual value
			prev_to_actual = beg;
		}
		
	}
}


void List::add_end(int val) {
	if (beg == NULL) // if list is empty
		add_beg(val);
	else {
		element* tmp = end; //save old end value
		end = new element;
		end->value = val;

		end->pointers_xor = reinterpret_cast<uintptr_t>(tmp) ^ reinterpret_cast<uintptr_t>(beg); //old_end ^ begin
		tmp->pointers_xor = (tmp->pointers_xor ^ reinterpret_cast<uintptr_t>(beg))^ reinterpret_cast<uintptr_t>(end); // update old end pointers_xor - prev_to_it ^ new_end
		beg->pointers_xor = reinterpret_cast<uintptr_t>(end) ^ (reinterpret_cast<uintptr_t>(tmp) ^ beg->pointers_xor); //update begin pointers_xot - new_end ^ next_to_begin

		if (prev_to_actual == tmp) { //if actual was 1st element assign prev_to_actual value
			prev_to_actual = end;
		}

	}
}


void List::add_act(int val) { //adding before actual element
	if (beg == NULL || actual == beg) // if list is empty or actual is begin
		add_beg(val);

	else {
			element* new_element = new element; //create new element so it will now be prev_to_actual
			new_element->value = val;

			actual->pointers_xor = (actual->pointers_xor ^ reinterpret_cast<uintptr_t>(prev_to_actual)) ^ reinterpret_cast<uintptr_t>(new_element); // next_to_actual ^ new_element (new prev_to_actual)
			prev_to_actual->pointers_xor= ( prev_to_actual->pointers_xor ^ reinterpret_cast<uintptr_t>(actual) ) ^ reinterpret_cast<uintptr_t>(new_element); //old prev_to_actual pointers_xor value - prev_to_prev_to_actual ^ new prev_to_actual
			new_element->pointers_xor = reinterpret_cast<uintptr_t>(prev_to_actual) ^ reinterpret_cast<uintptr_t>(actual); // old prev_to_actual ^ actual

			prev_to_actual = new_element;
		}
}


void List::delete_beg() {
	if (beg != NULL) { // no need to delete if list is empty 

		element* next_to_beg = reinterpret_cast<element*>(beg->pointers_xor ^ reinterpret_cast<uintptr_t>(end));

		if (beg == end) { //update end pointer if list has 1 element
			if (beg == actual)
				actual = NULL;
			end = NULL;
			delete_element(beg, next_to_beg, end);
			beg = NULL;
			

		}
		else {
			if (beg == actual) //update actual if it points to begin
				actual = next_to_beg;

			delete_element(beg, next_to_beg, end);
			beg = next_to_beg;
		}
	}
}


void List::delete_end() {
	if (end != NULL) { // no need to delete if list is empty 
		
		element* prev_to_end = reinterpret_cast<element*>(end->pointers_xor ^ reinterpret_cast<uintptr_t>(beg));

		if (end == beg) //update beg pointer if list has 1 element
			beg = NULL;
		if (end == actual) //update actual if it points to end
			actual = beg;

		
		delete_element(end, beg, prev_to_end);
		end = prev_to_end;

	}
}


void List::delete_actual() {
	if (actual == beg ) { 
		delete_beg();
		actual = end;
	}

	else if (actual == end) {
		delete_end();
		actual = end;
	}

	else { //if actual isn't begin or end

		element* next_to_actual = reinterpret_cast<element*>(actual->pointers_xor ^ reinterpret_cast<uintptr_t>(prev_to_actual));
		delete_element(actual, next_to_actual ,prev_to_actual);
		element* tmp = prev_to_actual;
		prev_to_actual = reinterpret_cast<element*>(prev_to_actual->pointers_xor ^ reinterpret_cast<uintptr_t>(actual)); //move prev_to_actual back
		actual = tmp;
	}
}


void List::del_val(int val) { //function will iterate through list and delete all the matching elements
	//if either begin or end match the value, delete them
	
	element* prev = end;
	element* current = beg; // starting from begin 
	while (current != NULL) { //ensure that list isnt empty
		if (current->value == val) {

			//handle all the special cases, continue after deleting in order not to miss any number 
			if (current == actual) {
				delete_actual();
				current = actual; //update current pointer;
				prev = prev_to_actual; //ensure correctness
				continue;
			}
			else if (current == beg) {
				delete_beg();
				current = beg;
				prev = end; //ensure correctness
				continue;
			}
			else if (current == end) {
				delete_end();
				current = end;
				prev = reinterpret_cast<element*>(end->pointers_xor ^ reinterpret_cast<uintptr_t>(beg)); // move prev to prev_to_end, so after delete_end() they don't both point at end
				continue;
			}
			else {
			element* tmp = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(prev) ^ current->pointers_xor); //move current to next to it;
			delete_element(current, NULL, NULL); //if current is not any of special cases, just delete it
			current = tmp;
			}
		}
		else {
			element* tmp = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(prev) ^ current->pointers_xor); //if current doesnt match the value move to the next
			prev = current;
			current = tmp;
		}

		if (current == beg) // break if function looped through whole list
			break;
	}
}

void List::delete_element(element* current, element* next_to_current, element* prev_to_current) { 
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




element* List::get_next(element* elem) { // function iterates from begin through whole list until it finds element passed as parameter and returns its previous element
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


element* List::get_prev(element* elem) { // function iterates from end through whole list until it finds element passed as parameter and returns its next element
	element* tmp = NULL;
	element* current = end;
	element* next = beg;
	while (next != elem) {
		tmp = current;
		if(current!=NULL)
			current = reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(next) ^ current->pointers_xor); //move current to previous to it
		next = tmp;
	}
	
	return current;

}


void List::next() { 
	if (actual != NULL) {
		element* tmp = prev_to_actual;
		prev_to_actual = actual;
		actual = reinterpret_cast<element*>(actual->pointers_xor ^ reinterpret_cast<uintptr_t>(tmp)); //move actual to next
		printf("%d\n", actual->value);
	}
	else
		printf("NULL\n");

}

void List::prev() {
	if (actual != NULL){

	element* tmp = actual;;
	actual = prev_to_actual;
	prev_to_actual = reinterpret_cast<element*>(prev_to_actual->pointers_xor ^ reinterpret_cast<uintptr_t>(tmp)); //move prev_to_actual to previous
	printf("%d\n", actual->value);
	}
	else
		printf("NULL\n");
}

void List::print_forward() const { //iterate through whole list from begin and print all elements
	element* prev = end;
	element* current = beg;
	if (beg == NULL)
		printf("NULL");
	while (current != NULL) {
		printf("%d ", current->value);
		element* tmp = current;
		current =  reinterpret_cast<element*>(reinterpret_cast<uintptr_t>(prev) ^ current->pointers_xor); //move current to next to it;
		prev = tmp;
		if (current == beg)
			break;
	}

	printf("\n");
}
void List::print_backward() const { //iterate through whole list from end and print all elements
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

List::~List() { //delete all the elements
	while (beg != NULL)
		delete_beg();
	end = NULL;
	actual = NULL;
	prev_to_actual = NULL;
}