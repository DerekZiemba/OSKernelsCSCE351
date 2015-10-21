#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "Resources.h"
#include "monitor.h"

#define HARD_DELAY 1000

/*positive 800 does a really good job of showing a full buffer 
 *negative 500 does pretty good at showing an empty buffer */
/*It's interesting to see not all threads will get equal time. For instance, 
* Moving the window around or doing some background task affects the buffer.*/
int ProducerBias = 800; //How much more often the producer will be run.

// Provided thread code
void producer(void *threadid) {
	long thread_id = (long)threadid;
	int iteration = 0;
	while (1) {		
		char alpha = rand_char();
		//printf("producer: ThreadID = %lu. Iteration = %d. BufferSize = %d\n", thread_id, iteration, CircularBuffer_OccupiedSpace(&ringbuf));
		mon_insert(alpha);
		hardDelay(HARD_DELAY);		
		iteration++;
	}
}

// Provided thread code
void consumer(void *threadid) {
	long thread_id = (long)threadid;
	int iteration = 0;
	while (1) {		
		char alpha = mon_remove(' ');
		printf("Removed Char %c\n", alpha);
		//printf("consumer: ThreadID = %lu. Iteration = %d. BufferSize = %d\n", thread_id, iteration, CircularBuffer_OccupiedSpace(&ringbuf));
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



