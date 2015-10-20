#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "Resources.h"
#include "monitor.h"

#define NUM_THREADS 8
#define BUFFER_SIZE 100
#define HARD_DELAY 1000

/*positive 800 does a really good job of showing a full buffer 
 *negative 500 does pretty good at showing an empty buffer */
/*It's interesting to see not all threads will get equal time. For instance, 
* Moving the window around or doing some background task affects the buffer.*/
int ProducerBias = 800; //How much more often the producer will be run.

CircularBuffer ringbuf;
sem_t full = NULL;
sem_t empty = NULL;
pthread_mutex_t mutex = NULL;



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
		//printf("consumer: ThreadID = %lu. Iteration = %d. BufferSize = %d\n", thread_id, iteration, CircularBuffer_OccupiedSpace(&ringbuf));
		hardDelay(HARD_DELAY);
		iteration++;
	}
}

    

int main() {
	//nextBufferIndex = 0;
	ringbuf = *CircularBuffer_init(BUFFER_SIZE);
	full = (sem_t*) malloc(sizeof(sem_t));
	empty = (sem_t*) malloc(sizeof(sem_t));
	mutex = (sem_t*) malloc(sizeof(sem_t));

	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	pthread_mutex_init(&mutex, 0);
	
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



