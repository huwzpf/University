#include "stos.h"
#include "stos.cpp"
#define SIZE 10000
int main() {
	Stack<const char*> stos;
	stos.Push("jebac disa");
	for (int i = 0; i < SIZE; i++) {
		stos.Push("jd");
	}
	
	for (int i = 0; i <= SIZE; i++) {
		cout<<stos.Pop()<<endl;
	}
	
	return 0;
}