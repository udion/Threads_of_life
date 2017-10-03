// The demo to show the updation of the common variable by multiple threads
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

//the function which threads will be running independently
void * runner(void *);
int counter = 0;
pthread_mutex_t mtx;

int main(){
	pthread_t myThreads[10];
	for(int i=0; i<10; i++){
		int ind = i;
		if(pthread_create(&myThreads[i], NULL, runner, &ind) != 0){
			printf("error in thread creation for thread %d", i);
		}
	}

	for(int i=0; i<10; i++){
		pthread_join(myThreads[i], NULL);
	}

	printf("value of counter in root thread: %d \n", counter);
}

void * runner(void * p){
	int * i = (int *) p;
	// //without locks, mutex
	// counter++;
	
	//with locks
	pthread_mutex_lock(&mtx);
	counter++;
	pthread_mutex_unlock(&mtx);

	printf("value of counter in thread %d: %d \n", *i, counter);
}