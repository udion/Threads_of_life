#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int taken;
pthread_cond_t cv;
pthread_mutex_t lock, l;

void* runner(void *);

int main(){
	//intialisation
	taken=0;
	pthread_cond_init(&cv, NULL);
	pthread_mutex_init(&lock, NULL);
	pthread_mutex_init(&l, NULL);

	//threads
	pthread_t th[3];

	//initialising the indices
	int ind[3];
	for(int i=0; i<3; i++){
		ind[i] = i;
	}

	for(int i=0; i<3; i++){
		pthread_create(&th[i], NULL, &runner, (void *)&ind[i]);
	}

	for(int i=0; i<3; i++){
		pthread_join(th[i],NULL);
	}
}

void * runner(void * a){
	pthread_mutex_lock(&l);
	if(taken==1){
		printf("An instance already running!\n");
	}
	else{
		taken=1;
	}
	pthread_mutex_unlock(&l);
	
	int *ind = (int *)a;
	pthread_mutex_lock(&lock);
	printf("Inside thread %d working...\n", *ind);
	sleep(1);
	printf("Thread %d finished.\n\n", *ind);
	taken = 0;
	pthread_mutex_unlock(&lock);
}