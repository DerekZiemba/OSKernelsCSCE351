#include "monitor.h"
#include "../SharedResources.h"

Monitor mon;
int bMonInited = 0;

Monitor *Monitor_init() {
	bMonInited = 1;
	
	Monitor *B = calloc(1, sizeof(Monitor));	
	B->queue = *RingBuff_init(BUFFER_SIZE);
	pthread_cond_init(&B->full, NULL);
	pthread_cond_init(&B->empty, NULL);
	pthread_mutex_init(&B->mutex, NULL);
	B->producers = calloc(NUM_THREADS, sizeof(pthread_t));
	B->consumers = calloc(NUM_THREADS, sizeof(pthread_t));
	return B;
}


void mon_insert(char alpha) {
	if (bMonInited == 0) {
		mon = *Monitor_init();
	}
	pthread_mutex_lock(&mon.mutex);
	
	int isFull = RingBuff_IsFull(&mon.queue);
	while(isFull) {
		pthread_cond_wait(&mon.full, &mon.mutex);		
		isFull = RingBuff_IsFull(&mon.queue);
	}
	
	RingBuff_Write(&mon.queue, alpha);
	RingBuff_PrintBuffer(&mon.queue);	
	
	if (RingBuff_IsEmpty(&mon.queue)) {
		pthread_cond_signal(&mon.empty);	
	}
	pthread_mutex_unlock(&mon.mutex);
	
}

char mon_remove(char replacementChar) {
	if (bMonInited==0) {
		printf("MONITOR NOT INITIALIZED");
	}
	pthread_mutex_lock(&mon.mutex);
	
	int isEmpty = RingBuff_IsEmpty(&mon.queue);
	while (isEmpty) {		
		pthread_cond_wait(&mon.empty, &mon.mutex);		
		isEmpty = RingBuff_IsEmpty(&mon.queue);
	}
	char value = RingBuff_Read(&mon.queue, ' ');
	
	if (RingBuff_IsFull(&mon.queue)) {
		pthread_cond_signal(&mon.empty);	
	}
	pthread_mutex_unlock(&mon.mutex);
	
	return value;
}