#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "..\SharedResources.h"

#define HARD_DELAY 1000

/*positive 800 does a really good job of showing a full buffer 
 *negative 500 does pretty good at showing an empty buffer */
/*It's interesting to see not all threads will get equal time. For instance, 
* Moving the window around or doing some background task affects the buffer.*/
int ProducerBias = 1500; //How much more often the producer will be run.

RingBuffer queue;

sem_t full = NULL;
sem_t empty = NULL;
sem_t mutex = NULL;


// Provided thread code
void *producer(void *threadid) {
	long thread_id = (long)threadid;
	int iteration = 0;
	while (1) {					
		sem_wait(&empty); 	
		sem_wait(&mutex);

		char x = 'X';//rand_char();
		queue.Write(&queue, x);
		if (PRINT_ADDITIONAL_INFO)
			printf("producer: ThreadID = %lu. Iteration = %d. BufferSize = %d\n", thread_id, iteration, RingBuffer_Count(&queue));
		
		if (PRINT_BUFFER_ON_INSERT)	
			queue.Print(&queue);
		
		sem_post(&mutex);  
		sem_post(&full);
		
		hardDelay(HARD_DELAY);
		iteration++;
	}
}

// Provided thread code
void *consumer(void *threadid) {
	long thread_id = (long)threadid;
	int iteration = 0;
	while (1) {		
		sem_wait(&full); 	
		sem_wait(&mutex);

		char elem = queue.Read(&queue, ' ');//REads the value and replaces it with an empty char. 
		if (PRINT_ADDITIONAL_INFO)
			printf("consumer: ThreadID = %lu. Iteration = %d. BufferSize = %d\n", thread_id, iteration, RingBuffer_Count(&queue));	
		
		if (PRINT_BUFFER_ON_REMOVE)	
			queue.Print(&queue);
		
		sem_post(&mutex);
		sem_post(&empty);  
		
		hardDelay(HARD_DELAY + ProducerBias);		
		iteration++;
	}
}

    

int main() {
	queue = *RingBuffer_init(BUFFER_SIZE);

	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&mutex, 0, 1);
	
	pthread_t producers[NUM_THREADS];
	pthread_t consumers[NUM_THREADS];
	
	long taskids[NUM_THREADS * 2];
	
	int i = 0;
	for (i = 0; i < NUM_THREADS; i++) {
		int id = i + NUM_THREADS;
		taskids[id] = id;	
		pthread_create(&consumers[i], NULL, &consumer, (void *) taskids[id]);		
	}
	
	for (i = 0; i < NUM_THREADS; i++) {
		taskids[i] = i;	
		pthread_create(&producers[i], NULL, &producer, (void *) taskids[i]);	
	}
	
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(producers[i], NULL);		
	}
    	
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(consumers[i], NULL);
	}
	return 0;
}



