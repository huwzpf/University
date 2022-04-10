#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include "pqueue.h"
#include <semaphore.h>
#include <fcntl.h>

char *filename = "queue.dat";
sem_t *A, *B;
int itemId = 0;

typedef struct item item;
struct item {
	int id;
};


item *
produce() {
	int time = rand() % 5 + 1;
	item *it = (item *)malloc(sizeof(item));
	sleep(time);
	it->id = itemId;
	itemId += 1;
	printf("produced %d\n", it->id);
	return it;
}

void
consume(item *it) {
	int time = rand() % 5 + 1;
	printf("consumed %d\n", it->id);
	sleep(time);
	free(it);
}


void
producer(pid_t childPid) {
	printf("producer started\n");
	pqueue *qu = NULL;
	item *it = NULL;
	while(1){
		it = produce();
		sem_wait(B);
		qunserialize(&qu, sizeof(item), filename);
		qinsert(&qu, it, it->id);
		qserialize(qu, sizeof(item), filename);		
		sem_post(A);
	}
}

void
consumer() {
	pqueue *qu = NULL;
	pqueue *qu_popped = NULL;
	item *it = NULL;
	printf("consumer started\n");
	
	while(1){
		sem_wait(A);
		qunserialize(&qu, sizeof(item), filename);
		if (qu != NULL){
		qserialize(qu, sizeof(item), filename);
		}
		sem_post(B);
		consume((item*)qpop(&qu)->data);
	}
}

int
main(int argc, char **argv) {
	pid_t pid;
	pqueue *qu = NULL;
	sem_unlink("/A");
	sem_unlink("/B");
	A = sem_open("/A", O_CREAT | O_EXCL, 0777, 0);

	B = sem_open("/B", O_CREAT | O_EXCL, 0777, 1);
	qserialize(qu, sizeof(item), filename);

	pid = fork();
	if(pid == 0){
		consumer();
	}
	
	else{
	  	producer(pid);
	}
	
	sem_unlink("/A");
	sem_unlink("/B");
	sem_close(A);
	sem_close(B);

	return 0;
}

