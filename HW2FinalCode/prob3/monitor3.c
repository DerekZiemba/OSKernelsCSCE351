#include "monitor3.h"
#include "../SharedResources.h"

Monitor mon;

/***************************************************************************
* Condition
****************************************************************************/
uint count(cond *self) {	return (self->nWaitingThreads);}

void cond_signal(cond* c) {
	if (c->nWaitingThreads) {
		c->nWaitingThreads--;
		sem_post(&mon.mutex);
		sem_post(&c->threadSemaphore);
		sem_wait(&mon.mutex);
	}
}

void cond_wait(cond* c) {
	c->nWaitingThreads++;
	sem_post(&mon.mutex);
	sem_wait(&c->threadSemaphore);
	sem_wait(&mon.mutex);		
}

cond* Cond_init() {
	cond* c = calloc(1, sizeof(c));
	//c->nWaitingThreads = *Queue_init();
	c->nWaitingThreads = 0;
	sem_init(&c->threadSemaphore, 0, 1);
	c->count = count;
	return c;
}


/***************************************************************************
* Monitor
****************************************************************************/
Monitor *Monitor_init() {
	Monitor *m = calloc(1, sizeof(Monitor));	
	
	m->queue = *RingBuffer_init(BUFFER_SIZE);
	m->full = *Cond_init();
	m->empty = *Cond_init();
	sem_init(&m->mutex,0,1);
	m->producers = calloc(NUM_THREADS, sizeof(pthread_t));
	m->consumers = calloc(NUM_THREADS, sizeof(pthread_t));
	m->bIsInitialized = true;
	
	return m;
}

void mon_insert(char alpha) {
	if(!mon.bIsInitialized)
		mon = *Monitor_init();
	
	sem_wait(&mon.mutex);	

	if(mon.queue.Count(&mon.queue) >= (mon.queue.size-NUM_THREADS)) {
		cond_wait(&mon.full);
	}
	
	mon.queue.Write(&mon.queue, alpha);
	
	if (PRINT_BUFFER_ON_INSERT)	
		mon.queue.Print(&mon.queue);

	sem_post(&mon.mutex);
}
	
char mon_remove(char replacementChar) {
	if (!mon.bIsInitialized)
		printf("MONITOR NOT INITIALIZED");
	
	sem_wait(&mon.mutex);	

	if (mon.queue.Count(&mon.queue) <= NUM_THREADS) {		
		cond_wait(&mon.empty);
	}

	//char value = (char*)Dequeue(&mon.queue);
	char value = mon.queue.Read(&mon.queue, ' ');
	
	cond_signal(&mon.full);
	sem_post(&mon.mutex);
	return value;
}




