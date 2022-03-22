#define _CRT_SECURE_NO_WARNINGS
#define BUF_SIZE 100
#include <iostream>
#include "List.h"

int main()
{

	List *list=new List;
	char input[BUF_SIZE] = {};
	
	while (scanf("%99s", input) != EOF) {
		if (!strcmp(input, "ADD_BEG")) {
			int num;
			scanf("%d", &num);
			list->add_beg(num);
		}
		else if (!strcmp(input, "ADD_END")) {
			int num;
			scanf("%d", &num);
			list->add_end(num);
		}
		else if (!strcmp(input, "ACTUAL")) {
			element* elem = list->get_actual();
			if (elem == NULL)
				printf("NULL\n");
			else
				printf("%d\n", elem->value);
		}
		else if (!strcmp(input, "NEXT")) {
			list->next();
		}
		else if (!strcmp(input, "PREV")) {
			list->prev();
		}
		else if (!strcmp(input, "DEL_BEG")) {
			list->delete_beg();
		}
		else if (!strcmp(input, "DEL_END")) {
			list->delete_end();
		}
		else if (!strcmp(input, "DEL_ACT")) {
			list->delete_actual();
		}
		else if (!strcmp(input, "PRINT_FORWARD")) {
			list->print_forward();
		}
		else if (!strcmp(input, "PRINT_BACKWARD")) {
			list->print_backward();
		}
		else if (!strcmp(input, "DEL_VAL")) {
			int num;
			scanf("%d", &num);
			list->del_val(num);
		}
		else if (!strcmp(input, "ADD_ACT")) {
			int num;
			scanf("%d", &num);
			list->add_act(num);
		}
		else break;
		
	}
	delete list;
	return 0;

}

