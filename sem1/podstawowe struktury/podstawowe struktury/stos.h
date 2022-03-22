#pragma once
#include "lista.h"
#include "lista.cpp"
template <class Type>
class Stack
{
public:
	Stack();
	Type Pop();
	void Push(Type arg);
	bool Is_empty();
private:
	List<Type>* list;
};

