#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5

void* runner (void *);

sem_t *spoon;
sem_t num_enter;

int main(){
	//initialisation
	spoon = (sem_t *) malloc(sizeof(sem_t)*N);
	for(int i=0; i<N; i++){
		sem_init(&spoon[i], 0, 1);
	}
	sem_init(&num_enter, 0, N-1);
	int ind[N];
	for(int i=0; i<N; i++){
		ind[i] = i;
	}
	pthread_t th[N];

	for(int i=0; i<N; i++){
		pthread_create(&th[i], NULL, &runner, (void *)&ind[i]);
	}
	for(int i=0; i<N; i++){
		pthread_join(th[i], NULL);
	}
}

void* runner(void * a){
	int *ind = (int *)a;
	sem_wait(&num_enter);
	sem_wait(&spoon[*ind]);
	printf("Philospher %d: got left spoon!\n", *ind);
	sem_wait(&spoon[(*ind+1)%N]);
	printf("Philospher %d: got right spoon!\n", *ind);
	printf("Philospher %d: eating...\n", *ind);
	sleep(1);
	printf("Philospher %d: thinking!\n", *ind);
	sem_post(&spoon[(*ind+1)%N]);
	sem_post(&spoon[*ind]);
	sem_post(&num_enter);
}