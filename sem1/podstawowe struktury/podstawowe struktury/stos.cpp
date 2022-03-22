#include "stos.h"

template <class Type>
Type Stack<Type>::Pop() {
	if (!list->is_empty()) {
		Type ret = list->get_beg();
		list->delete_first();
	return (ret);
}
	else {

		cout << "STRUKTURA PUSTA" << endl;
		return NULL;
	}
}
template <class Type>
void Stack<Type>::Push(Type arg) {
	list->insert_first(arg);
}

template <class Type>
bool Stack<Type>::Is_empty() {
	return list->is_empty();
}

template <class Type>
Stack<Type>::Stack() {
	list = new List<Type>;
}