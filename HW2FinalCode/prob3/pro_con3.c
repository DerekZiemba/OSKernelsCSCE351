#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../SharedResources.h"
#include "monitor3.h"

#define HARD_DELAY 800

// Provided thread code
void *producer(void *threadid) {
	long thread_id = (long)threadid;
	int iteration = 0;
	while (1) {		
		char alpha = rand_alpha();
		//printf("producer: ThreadID = %lu. Iteration = %d. BufferSize = %d\n", thread_id, iteration, RingBuff_OccupiedSpace(&ringbuf));
		//printf("ADDING Char %c, ThreadID: %lu, Iteration: %d\n", alpha,thread_id,iteration);
		mon_insert(alpha);
		hardDelay(HARD_DELAY);		
		iteration++;
	}
}

// Provided thread code
void *consumer(void *threadid) {
	long thread_id = (long)threadid;
	int iteration = 0;
	while (1) {		
		char alpha = mon_remove(' ');
		//printf("REMOVED Char %c, ThreadID: %lu, Iteration: %d\n", alpha, thread_id, iteration);
		if (PRINT_ADDITIONAL_INFO)
			printf("removed char: %c\n", alpha);
		//printf("consumer: ThreadID = %lu. Iteration = %d. BufferSize = %d\n", thread_id, iteration, RingBuff_OccupiedSpace(&ringbuf));
		hardDelay(HARD_DELAY);
		iteration++;
	}
}

    

int main() {

	int numThreads = NUM_THREADS * 2;
	
	pthread_t threads[numThreads];
	long taskids[numThreads];
	
	int i = 0;
	for (i = 0; i < numThreads; i++) {
		taskids[i] = i;	
		if ((i % 2) == 0)
			pthread_create(&threads[i], NULL, &producer, (void *) taskids[i]);	
		else
			pthread_create(&threads[i], NULL, &consumer, (void *) taskids[i]);				
	}
	
	for (i = 0; i < numThreads; i++) {
		pthread_join(threads[i], NULL);					
	}

	return 0;
}



