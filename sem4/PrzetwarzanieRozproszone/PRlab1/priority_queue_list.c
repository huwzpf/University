#include <stdio.h>
#include <stdlib.h>
#include "priority_queue_list.h"

void
qlist(pqueue* head, void (*print_data)(void*)) {
	pqueue* p;

	for (p = head; p != NULL; p = p->next) {
		printf("%d: ", p->k);
		print_data(p->data);
		printf("\n");
	}

}

void
qinsert(pqueue** phead, void* data, int k) {
	if (*phead == NULL) {
		*phead = (pqueue*)malloc(sizeof(pqueue));
		if (*phead != NULL) {
			(*phead)->k = k;
			(*phead)->data = data;
			(*phead)->next = NULL;
			(*phead)->prev = NULL;
		}
		else {
			printf("ERROR");
		}
	}
	else {
		pqueue* t = *phead;
		pqueue* new_elem = (pqueue*)malloc(sizeof(pqueue));
		while (t->k > k && t->next != NULL)
			t = t->next;
		new_elem->data = data;
		new_elem->k = k;
		if (t == *phead && k > t->k) {
			new_elem->next = t;
			t->prev = new_elem;
			new_elem->prev = NULL;
			*phead = new_elem;
		}
		else {
			new_elem->prev = t;
			new_elem->next = t->next;
			if (t->next != NULL)
				t->next->prev = new_elem;
			t->next = new_elem;
		}

	}
}


void
qremove(pqueue** phead, int k) {
	pqueue* t = *phead;
	while (t->k != k) {
		t = t->next;
		if (t->next == NULL) {
			if (t->k == k) {
				break;
			}
			else {
				return;
			}
		}
	}
	while (t->k == k) {
		if (t == *phead) {
			*phead = t->next;
		}
		else {
			t->prev->next = t->next;
		}
		if (t->next != NULL)
			t->next->prev = t->prev;
		free(t);
	}
}