#include  <stdio.h>
#include  <pthread.h>
#define SIZE 120
#define THREADS_NUM 2


int idx = 0;
pthread_mutex_t m;

int is_prime (int num){
	if (num == 0 || num == 1)
		return 0;
	for (int i = 2; i < num; ++i){
	if(num%i == 0)
		return 0;
	}
	return  1;
}
int arr[SIZE];

void *f(void * i)
{

int current = 0;
	while (idx < SIZE){
		pthread_mutex_lock(&m);
		current = idx;
		if(idx < SIZE)
			idx += 1;
		pthread_mutex_unlock(&m);		
		if(is_prime(arr[current])){
			printf("%d is prime\n", arr[current]);
		}
	
	}
pthread_exit(NULL);
}

int  main()
{
for(int i =0; i< SIZE; i++){
	arr[i] = i;
}

pthread_mutex_init(&m, NULL);

pthread_t w[THREADS_NUM];
for(int i =0; i< THREADS_NUM; i++){
 	pthread_create (&(w[i]), NULL , f, NULL);
}

for(int i =0; i< THREADS_NUM; i++){
 	pthread_join (w[i], NULL);

}
pthread_mutex_destroy(&m);
}
