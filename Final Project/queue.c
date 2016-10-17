/**
  * --------------------------FINAL PROJECT----------------------------------
  * @author Sneha Dipankar Roy (sr0069@uah.edu)
  * @class CS590-01 Summer 2016
  * @date 9th August 2016
  * @desc C program that creates a number of producer and consumer threads
  *	  using the pthread library
  * -------------------------------------------------------------------------
  */

// Header files
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Global variable declaration
int circqueue[100]; // Circular queue as an array
int read, write;
pthread_t prod[100], cons[100]; // Producer & consumer threads
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER; // Mutex variable
pthread_cond_t prodc = PTHREAD_COND_INITIALIZER; // Condition variable
pthread_cond_t consc = PTHREAD_COND_INITIALIZER; // Condition variable
unsigned int seed = 200; // rand_r() seed initialization

void sleepy() {
	// Variable declaration
	int sleepTime;
	seed++;
	// Usage of rand_r() i.e. thread-safe function to generate randum number
	sleepTime = rand_r(&seed) % 3;
        sleepTime++;
        sleep(sleepTime);
}

void *
producer(void *arg) {
	// Variable declaration & initialization
	int randumNum;
	pthread_t tid = pthread_self();
	while (1) {
        	// Usage of rand_r() i.e. thread-safe function to generate randum numbers
		randumNum = rand_r(&seed) % 1000;
        	randumNum++;
		seed++;
		// Lock circqueue 
		pthread_mutex_lock(&mymutex);
		// Check if circqueue is full 
		while (circqueue[write] != 0) {
			printf("Queue is full. \n");
			// Block until more space is available
			pthread_cond_wait(&consc, &mymutex);
		}
		// Place item on circqueue	
		circqueue[write] = randumNum;
		// Print message regarding placement
		printf("Producer %lu added %d to slot %d \n", tid, circqueue[write], write + 1);
		write = (write+1)%100;
		// Release circqueue
		pthread_mutex_unlock(&mymutex);
		// Wake up consumer
		pthread_cond_signal(&prodc);
		// Sleep for a random amount of time
		sleepy();
	}
}

void *
consumer(void *arg) {
	// Variable declaration & initialization
	pthread_t tid = pthread_self();
	while (1) {
		// Lock circqueue
		pthread_mutex_lock(&mymutex);
		// Check if circqueue is empty
		while (circqueue[read] == 0) {
			printf("Queue is empty. \n");
			// Block until an item has been added
			pthread_cond_wait(&prodc, &mymutex);
		}
		// Print message regarding removal
		printf("Consumer %lu removed %d from slot %d \n", tid, circqueue[read], read + 1);
		// Remove item on circqueue
		circqueue[read] = 0;
		read = (read+1)%100;
		// Release circqueue
		pthread_mutex_unlock(&mymutex);
		// Wake up producer
		pthread_cond_signal(&consc);
		// Sleep for a random amount of time
                sleepy();
	}
}

int main() {
	// Variable declaration
	int i, noOfProd, noOfCons;
	// Variable initialization
	read = 0;
	write = 0;
	// Place 0 on all slots of circqueue
	for (i = 0; i < 100; i++)
		circqueue[i] = 0;
	// Prompt user to enter number of producer and consumer threads
	printf("Enter the number of producer threads to be created: \n");
	scanf("%d", &noOfProd);
	printf("Enter the number of consumer threads to be created: \n");
	scanf("%d", &noOfCons);
	// Create producer threads
	for (i = 0; i < noOfProd ; i++)	{
		pthread_create(&prod[i], NULL, producer, NULL);
	}
	// Create consumer threads
	for (i = 0; i < noOfCons ; i++)  {
                pthread_create(&cons[i], NULL, consumer, NULL);
        }
	// Wait for prod[0] termination
	pthread_join(prod[0], NULL);
	// Destroy mutex variable
	pthread_mutex_destroy(&mymutex);
	return 0;
}
