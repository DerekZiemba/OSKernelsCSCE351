#include "monitor.h"
#include "Resources.h"

Monitor *Monitor_init() {
	Monitor *B = calloc(1, sizeof(Monitor));	
	B->queue = CircularBuffer_init(BUFFER_SIZE);
	sem_init(&B->full, 0, 0);
	sem_init(&B->empty, 0, BUFFER_SIZE);
	sem_init(&B->mutex, 0, 1);
	B->count = 0;
	B->
	pthread_t producers[NUM_THREADS];
	pthread_t consumers[NUM_THREADS];
	return B;
}

void mon_insert(char alpha) {
	
}

char mon_remove(char replacementChar) {
	
}