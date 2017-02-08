#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<pthread.h>

#define NUM_READS  5
#define NUM_WRITES 5
#define NUM_READERS  5
#define NUM_WRITERS 5

static unsigned int gSharedValue = 0;
static int gReaderCount = 0, gWaitingReaderCount = 0;

pthread_mutex_t gSharedMemLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gReadPhase = PTHREAD_COND_INITIALIZER;
pthread_cond_t gWritePhase = PTHREAD_COND_INITIALIZER;

void *readerMain(void *arg);
void *writerMain(void *arg);

int main() 
{
   int read_threadNum[NUM_READERS]; 
   int write_threadNum[NUM_WRITERS]; 

   pthread_t read_thread[NUM_READERS];
   pthread_t write_thread[NUM_WRITERS];

   srand(time(NULL));

   for (int i = 0; i < NUM_READERS; i++) {
    read_threadNum[i] = i;
    pthread_create(&read_thread[i], NULL, readerMain, &read_threadNum[i]);
   }

   for (int i = 0; i < NUM_WRITERS; i++) {
    write_threadNum[i] = i;
    pthread_create(&write_thread[i], NULL, writerMain, &write_threadNum[i]);
   }

   for (int i = 0; i < NUM_READERS; i++) {
   	pthread_join(read_thread[i], NULL);
   }

   for (int i = 0; i < NUM_WRITERS; i++) {
   	pthread_join(write_thread[i], NULL);
   }

   return 0;
}

/******************/
static void read_lock(int *numReaders)
{
	pthread_mutex_lock(&gSharedMemLock);
	gWaitingReaderCount++;
	while (gReaderCount == -1)
		pthread_cond_wait(&gReadPhase, &gSharedMemLock);
	gWaitingReaderCount--;
	*numReaders = ++gReaderCount;
	pthread_mutex_unlock(&gSharedMemLock);
}
static void read_unlock()
{
	pthread_mutex_lock(&gSharedMemLock);
	gReaderCount--;
	if (gReaderCount == 0)
		pthread_cond_signal(&gWritePhase);
	pthread_mutex_unlock(&gSharedMemLock);
}

static void write_lock(int *numReaders)
{
	pthread_mutex_lock(&gSharedMemLock);
	while (gReaderCount != 0)
		pthread_cond_wait(&gWritePhase, &gSharedMemLock);
	gReaderCount = -1;
	*numReaders = gReaderCount;
	pthread_mutex_unlock(&gSharedMemLock);
}

static void write_unlock()
{
	pthread_mutex_lock(&gSharedMemLock);
	gReaderCount = 0;
	if (gWaitingReaderCount > 0)
		pthread_cond_broadcast(&gReadPhase);
	else 
		pthread_cond_signal(&gWritePhase);
	pthread_mutex_unlock(&gSharedMemLock);
}
/******************/
void *readerMain(void *arg) {
	int id = *((int *)arg);
	int numReaders = 0;

	for (int i = 0; i < NUM_READS; i++) {
		sleep(rand() % NUM_READERS + NUM_WRITERS);
		read_lock(&numReaders);
		printf("ReadId: %d Value: %d ReaderCount: %d\n", 
				id, gSharedValue, numReaders);
		read_unlock();
	}
	pthread_exit(0);
}

void *writerMain(void *arg) {
	int id = *((int *)arg);
	int numReaders = 0;

	for (int i = 0; i < NUM_WRITES; i++) {
		sleep(rand() % NUM_READERS + NUM_WRITERS);
		write_lock(&numReaders);
		printf("WriteId: %d Value: %d ReaderCount: %d\n", 
				id, ++gSharedValue, numReaders);
		write_unlock();
	}
	pthread_exit(0);
}
