// Shared buffer producer consumer problem, with limited buffer size

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define buffsize 100
#define numProd 200
#define numCons 50

int sharedBuff;

sem_t sp;
sem_t sc;
sem_t lock;

void *producer(void *);
void *consumer(void *);

int main(){
	//many producer many consumer
	pthread_t prod[numProd];
	pthread_t cons[numCons];

	sem_init(&sp, 0, buffsize);
	sem_init(&sc, 0, 0);
	sem_init(&lock,0,1);

	sharedBuff = 0;

	printf("inside main\n");

	for(int i=0; i<numProd; i++){
		int ind = i;
		pthread_create(&prod[i], NULL, producer, &ind);
	}

	for(int i=0; i<numCons; i++){
		int ind = i;
		pthread_create(&cons[i], NULL, consumer, &ind);
	}
	
	for(int i=0; i<numProd; i++){
		pthread_join(prod[i], NULL);
	}

	for(int i=0; i<numCons; i++){
		pthread_join(cons[i], NULL);
	}
}

void *producer(void * i){
	//printf("inside producer\n");
	int * p = (int *)i;
	sem_wait(&sp);
	sem_wait(&lock);
	sharedBuff++;
	printf("I am producer %d and I produced. Now: %d\n", *p, sharedBuff);
	sem_post(&lock);
	sem_post(&sc);
}

void *consumer(void * i){
	//printf("inside consumer\n");
	int *c = (int *)i;
	sem_wait(&sc);
	sem_wait(&lock);
	sharedBuff--;
	printf("I am consumer %d and I consumed. Now: %d\n", *c, sharedBuff);
	sem_post(&lock);
	sem_post(&sp);
}