#include "monitor2.h"
#include "../SharedResources.h"

Monitor mon;

Monitor *Monitor_init() {
	Monitor *m = calloc(1, sizeof(Monitor));	
	
	//m->queue = *RingBuffer_init(BUFFER_SIZE);
	m->queue = *Queue_init(BUFFER_SIZE);
	pthread_cond_init(&m->full, NULL);
	pthread_cond_init(&m->empty, NULL);
	pthread_mutex_init(&m->mutex, NULL);
	m->producers = calloc(NUM_THREADS, sizeof(pthread_t));
	m->consumers = calloc(NUM_THREADS, sizeof(pthread_t));
	m->bIsInitialized = true;
	
	return m;
}


void mon_insert(char alpha) {	
	if (!mon.bIsInitialized)
		mon = *Monitor_init();
	
	pthread_mutex_lock(&mon.mutex);
	if (mon.queue.count >= (mon.queue.maxsize)) {
	//if (mon.queue.Count(&mon.queue) >= (mon.queue.size-NUM_THREADS)) {
		//Broadcast to threads that are currently suspended and waiting on the full condition they can start
		pthread_cond_signal(&mon.full); 
		//Suspend this thread until the threads that just started broadcast the empty condition. 
		pthread_cond_wait(&mon.empty, &mon.mutex);	
	}
	//mon.queue.Write(&mon.queue, alpha);
	
	Queue_Enqueue(&mon.queue, (void*) alpha);
	if (PRINT_BUFFER_ON_INSERT) {
		//mon.queue.Print(&mon.queue);
		Queue_Print(&mon.queue, "%c");
	}
			
	pthread_mutex_unlock(&mon.mutex);
}
	
char mon_remove(char replacementChar) { 
	if (!mon.bIsInitialized)
		printf("MONITOR NOT INITIALIZED");
	
	pthread_mutex_lock(&mon.mutex);

	//if (mon.queue.Count(&mon.queue) <= NUM_THREADS) {		
	if (mon.queue.count <= 0) {	
		pthread_cond_signal(&mon.empty); //Tell the threads that are waiting on empty condition to start
		pthread_cond_wait(&mon.full, &mon.mutex);//Wait for those threads to broadcast the full condition.	
	}
	
	//char value = mon.queue.Read(&mon.queue, ' ');
	char value = (char*) Queue_Dequeue(&mon.queue);
	
	if (PRINT_BUFFER_ON_REMOVE) {
		//mon.queue.Print(&mon.queue);
		Queue_Print(&mon.queue, "%c");
	}
		
	
	pthread_mutex_unlock(&mon.mutex);

	return value;
}