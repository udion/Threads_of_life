#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define ITER 30

void* runner(void *);

sem_t l1;
sem_t l2;
int counter;

int main(){
	sem_init(&l1, 0, 1);
	sem_init(&l2, 0, 0);

	pthread_t th[2];
	int ind[2] = {0,1};

	for(int i=0; i<2; i++){
		pthread_create(&th[i], NULL, &runner, (void *)&ind[i]);
	}
	for(int i=0; i<2; i++){
		pthread_join(th[i], NULL);
	}
}

void * runner(void *a){
	int *ind = (int *)a;
	for(int i=0; i<ITER; i++){
		if(*ind == 0){
			sem_wait(&l1);
		}
		else{
			sem_wait(&l2);
		}
		printf("Hi, I am thread %d\n", *ind);
		sleep(0.5);
		if(*ind == 0){
			sem_post(&l2);
		}
		else{
			sem_post(&l1);
		}
	}
}