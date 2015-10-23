#include "monitor2.h"
#include "../SharedResources.h"

Monitor mon;
bool bMonInited = false;


Monitor *Monitor_init() {
	bMonInited = true;
	
	Monitor *B = calloc(1, sizeof(Monitor));	
	B->queue = *RingBuffer_init(BUFFER_SIZE);
	pthread_cond_init(&B->full, NULL);
	pthread_cond_init(&B->empty, NULL);
	pthread_mutex_init(&B->mutex, NULL);
	B->producers = calloc(NUM_THREADS, sizeof(pthread_t));
	B->consumers = calloc(NUM_THREADS, sizeof(pthread_t));
	return B;
}


void mon_insert(char alpha) {	
	if (!bMonInited) 
		mon = *Monitor_init();
	
	pthread_mutex_lock(&mon.mutex);
		
	if (RingBuffer_IsFull(&mon.queue)) {
		//Broadcast to threads that are currently suspended and waiting on the full condition they can start
		pthread_cond_broadcast(&mon.full); 
		//Suspend this thread until the threads that just started broadcast the empty condition. 
		pthread_cond_wait(&mon.empty, &mon.mutex);	
	}
	
	RingBuffer_Write(&mon.queue, alpha);
	RingBuffer_Print(&mon.queue);	
	
	pthread_mutex_unlock(&mon.mutex);
	
}
	
char mon_remove(char replacementChar) {
	if (!bMonInited) 
		printf("MONITOR NOT INITIALIZED");
	
	pthread_mutex_lock(&mon.mutex);

	if (RingBuffer_IsEmpty(&mon.queue)) {		
		pthread_cond_broadcast(&mon.empty); //Tell the threads that are waiting on empty condition to start
		pthread_cond_wait(&mon.full, &mon.mutex);//Wait for those threads to broadcast the full condition.	
	}
	
	char value = RingBuffer_Read(&mon.queue, ' ');
	pthread_mutex_unlock(&mon.mutex);

	return value;
}