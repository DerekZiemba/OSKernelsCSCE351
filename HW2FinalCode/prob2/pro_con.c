#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../SharedResources.h"
#include "monitor.h"

#define HARD_DELAY 1000

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
		printf("removed char: %c\n", alpha);
		//printf("consumer: ThreadID = %lu. Iteration = %d. BufferSize = %d\n", thread_id, iteration, RingBuff_OccupiedSpace(&ringbuf));
		hardDelay(HARD_DELAY);
		iteration++;
	}
}

    

int main() {

	pthread_t producers[NUM_THREADS];
	pthread_t consumers[NUM_THREADS];
	
	long taskids[NUM_THREADS * 2];
	
	int i = 0;
	for (i = 0; i < NUM_THREADS; i++) {
		taskids[i] = i;	
		pthread_create(&producers[i], NULL, &producer, (void *) taskids[i]);	
	}
	for (i = 0; i < NUM_THREADS; i++) {
		int id = i + NUM_THREADS;
		taskids[id] = id;	
		pthread_create(&consumers[i], NULL, &consumer, (void *) taskids[id]);		
	}
	
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(producers[i], NULL);		
	}
    	
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(consumers[i], NULL);
	}
	return 0;
}



