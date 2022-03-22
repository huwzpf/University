#include "lista.h"

template <class Type>
List<Type>::List() {
	beg = nullptr;
	end = nullptr;
	content = NULL;
}

template <class Type>
void List<Type>::insert_after(element* arg, Type cont) {
	element* temp = arg;
	arg = new element;
	arg->next = temp->next;
	arg->prev = temp;
	arg->content = cont;
	temp->next = arg;
}

template <class Type>
void List<Type>::delete_last() {
	if (end!= nullptr && end->prev != nullptr) {
		end = end->prev;
		delete(end->next);
		end->next = nullptr;
	}
	else if (end->prev == nullptr) {
		delete(end);
		end = nullptr;
	}
}
template <class Type>
void List<Type>::delete_first() {
	if (beg != nullptr && beg->next!=nullptr) {
	beg = beg->next;
	delete (beg->prev);
	beg->prev = nullptr;
}
	else if (beg->next == nullptr) {
		delete (beg);
		beg = nullptr;
	}
}

template <class Type>
void List<Type>::insert_first(Type arg) {
	element* temp = beg;
	if (temp!=nullptr) {
		if (temp->next == nullptr)
			end = temp;
	}
	beg = new element;
	beg->content = arg;
	beg->prev = nullptr;
	if (temp != nullptr) {
		temp->prev = beg;
	}
	beg->next = temp;
	
}

template <class Type>
void List<Type>::insert_last(Type arg) {
	if (beg == nullptr) {
		insert_first(arg);
	}
	else if (end == nullptr) {
		end = new element;
		end->content = arg;
		end->next = nullptr;
		end->prev = beg;
	}
	else {
		element* temp = end;
		end = new element;
		end->prev = temp;
		end->content = arg;
		end->next = nullptr;
		temp->next = end;
		
		
	}
}

template <class Type>
void List<Type>::print() {
	element* temp = beg;
	while (temp != nullptr) {
		cout << temp->content << endl;
		temp = temp->next;
	}
}

