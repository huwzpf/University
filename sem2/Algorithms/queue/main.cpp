#define _CRT_SECURE_NO_WARNINGS
#define BUF_SIZE 100
#include <iostream>
#include "Queue.h"

int main()
{


	Queue* queue = new Queue;
	char input[BUF_SIZE] = {};

	while (scanf("%99s", input) != EOF) {
		if (!strcmp(input, "ADD_BEG")) {
			int num;
			scanf("%d", &num);
			queue->add_beg(num);
		}
		else if (!strcmp(input, "ADD_END")) {
			int num;
			scanf("%d", &num);
			queue->add_end(num);
		}
		else if (!strcmp(input, "DEL_BEG")) {
			queue->delete_beg();
		}
		else if (!strcmp(input, "DEL_END")) {
			queue->delete_end();
		}
		else if (!strcmp(input, "PRINT_FORWARD")) {
			queue->print_forward();
		}
		else if (!strcmp(input, "PRINT_BACKWARD")) {
			queue->print_backward();
		}
		else if (!strcmp(input, "PRINT_QUEUE")) {
			queue->print_queue();
		}
		else if (!strcmp(input, "COUNT")) {
			printf("%d\n",queue->get_count());
		}
		else if (!strcmp(input, "SIZE")) {
			printf("%d\n",queue->get_size());
		}
		else if (!strcmp(input, "POP")) {
			queue->pop();
		}
		else if (!strcmp(input, "GARBAGE_SOFT")) {
			queue->garbage_soft();
		}
		else if (!strcmp(input, "GARBAGE_HARD")) {
			queue->garbage_hard();
		}
		else if (!strcmp(input, "PUSH")) {
			int num;
			scanf("%d", &num);
			queue->push(num);
		}
		else break;

	}
	delete queue;
	return 0;

}

