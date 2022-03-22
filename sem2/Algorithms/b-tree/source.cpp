#define _CRT_SECURE_NO_WARNINGS
#include "Tree.h"
#include <iostream>


using namespace std; 
int main() {
	
	char c;
	Tree* t=nullptr;
	FILE* in = stdin;

	
	while (fscanf(in,"%c", &c)) {


		if (c == 'I') {
			int number;
			fscanf(in, "%d", &number);
			t = new Tree(number);

		}
		else if (c == 'A') {

			int number;
			fscanf(in, "%d", &number);
			t->insert(number);
		}
		else if (c == '?') {
			int number;
			fscanf(in, "%d", &number);
			t->search_value(number) ? printf("%d +\n", number) : printf("%d -\n", number);
		}
		else if (c == 'L') {
			int order;
			fscanf(in, "%d", &order); //read tree's order from input 
			t = new Tree(order); //create tree and start loading it
			t->construct_tree();

		}
		else if (c == 'S') {
			t->print(stdout, false);
			printf("\n");
		}
		else if (c == 'P') {
			t->print(stdout, true);
			printf("\n");
		}

		else if (c == 'R') {

			int number;
			fscanf(in, "%d", &number);
			t->remove(number);

		}
		else if (c == 'C') {
			t->calculate_cache(in);
		}
		else if (c == '#') {
			continue;
		}
		else if (c == 'X') {
			return 0;
		}
	}

	if (t != nullptr)
		delete t;
	
	return 0;
}



