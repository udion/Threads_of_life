#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

long int reader_count;
long int writer_count;

long int reader_loop;
long int writer_loop;

int rwlock_enabled=1;

struct read_write_lock
{
	pthread_mutex_t mtx;
	pthread_cond_t cond;
	int readers;
	int writers;
};

struct read_write_lock rwlock;
pthread_spinlock_t spinlock;

long int data=0;			//	Shared data variable

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
	//	Write the code for initializing your read-write lock.
	rw->readers = 0;
	rw->writers = 0;
	pthread_cond_init(&rw->cond, NULL);
	pthread_mutex_init(&rw->mtx, NULL);
}

void ReaderLock(struct read_write_lock * rw)
{
	//	Write the code for aquiring read-write lock by the reader.
	pthread_mutex_lock(&rw->mtx);
	while(rw->writers){
		pthread_cond_wait(&rw->cond, &rw->mtx);
	}
	rw->readers++;
	pthread_mutex_unlock(&rw->mtx);
}

void ReaderUnlock(struct read_write_lock * rw)
{
	//	Write the code for releasing read-write lock by the reader.
	pthread_mutex_lock(&rw->mtx);
	rw->readers--;
	pthread_cond_broadcast(&rw->cond);
	pthread_mutex_unlock(&rw->mtx);
}

void WriterLock(struct read_write_lock * rw)
{
	//	Write the code for aquiring read-write lock by the writer.
	pthread_mutex_lock(&rw->mtx);
	while(rw->writers || rw->readers){
		pthread_cond_wait(&rw->cond, &rw->mtx);
	}
	rw->writers++;
	pthread_mutex_unlock(&rw->mtx);
}

void WriterUnlock(struct read_write_lock * rw)
{
	//	Write the code for releasing read-write lock by the writer.
	pthread_mutex_lock(&rw->mtx);
	rw->writers--;
	pthread_cond_broadcast(&rw->cond);
	pthread_mutex_unlock(&rw->mtx);
}




void *ReaderFunction(void *arg)
{
	int threadNUmber = *((int *)arg);

	for(int i=0;i<reader_loop;i++)
	{
		//	Acquiring lock
		if(rwlock_enabled)
			ReaderLock(&rwlock);
		else
			pthread_spin_lock(&spinlock);

		printf("Reader: %2d, value: %ld\n",threadNUmber, data);
				
		// Release lock
		if(rwlock_enabled)
			ReaderUnlock(&rwlock);
		else
			pthread_spin_unlock(&spinlock);

		usleep(900);

	}
}

void *WriterFunction(void *arg)
{

	int threadNUmber = *((int *)arg);

	for(int i=0;i<writer_loop;i++)
	{
		//	Acquiring lock
		if(rwlock_enabled)
			WriterLock(&rwlock);
		else
			pthread_spin_lock(&spinlock);

		data++;
		printf("Writer: %2d, value: %ld\n",threadNUmber, data);
		
		// Release lock
		if(rwlock_enabled)
			WriterUnlock(&rwlock);
		else
			pthread_spin_unlock(&spinlock);

		usleep(900);
	}
	
}

int main(int argc, char *argv[])
{
	pthread_t *writer_threads;
	pthread_t *reader_threads;
	int *threadNUmber;

	struct args *arg;
	time_t start, stop;

	if(argc<6)
	{
		printf("read-write <readers-count> <writers-count> <reader-loop-size> <writer-loop-size> <enable/disable rwlock>\n");
		exit(1);
	}

	reader_count=atol(argv[1]);
	writer_count=atol(argv[2]);

	reader_loop=atol(argv[3]);
	writer_loop=atol(argv[4]);

	rwlock_enabled=atoi(argv[5]);
	

	//	Initlizing spin lock and rwlock;
	pthread_spin_init(&spinlock,0);   //spinlock used only if rwlock is disabled

	InitalizeReadWriteLock(&rwlock);

	time(&start);

	writer_threads = (pthread_t *) malloc(writer_count * sizeof(pthread_t));
	reader_threads = (pthread_t *) malloc(reader_count * sizeof(pthread_t));

	for(int i=0;i<writer_count; i++)
	{		

		//	Create a writer thread. 
		//	use writer_threads array to map pthread_t variable for each thread
		//	writer thread number needs to be send to the writer function
		int ind = i;
		pthread_create(&writer_threads[i], NULL, WriterFunction, &ind);
	}

	for(int i=0;i<reader_count; i++)
	{

		//	Create a reader thread. 
		//	use reader_threads array to map pthread_t variable for each thread
		//	reader thread number needs to be send to the reader function
		int ind = i;
		pthread_create(&reader_threads[i], NULL, ReaderFunction, &ind);
	}

	for(int i=0;i<writer_count; i++)
		pthread_join(writer_threads[i],NULL);

	for(int i=0;i<reader_count; i++)
		pthread_join(reader_threads[i],NULL);

	time(&stop);
   	printf("Finished in about %.0f seconds. \n", difftime(stop, start));

	exit(1);
}
