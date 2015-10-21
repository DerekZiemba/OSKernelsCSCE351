#include "monitor.h"
#include "Resources.h"

Monitor mon;
int bMonInited = 0;

Monitor *Monitor_init() {
	bMonInited = 1;
	
	Monitor *B = calloc(1, sizeof(Monitor));	
	B->queue = *CircularBuffer_init(BUFFER_SIZE);
	sem_init(&B->mutex, 0, 1);
	pthread_cond_init(&B->full, NULL);
	pthread_cond_init(&B->empty, NULL);
	pthread_mutex_init(&B->count_mutex, NULL);
	pthread_cond_init(&B->count_threshold_cv, NULL);
	B->count = 0;
	B->producers = calloc(NUM_THREADS, sizeof(pthread_t));
	B->consumers = calloc(NUM_THREADS, sizeof(pthread_t));
	B->taskids = calloc(NUM_THREADS, sizeof(long));
	return B;
}


void mon_insert(char alpha) {
	if (bMonInited == 0) {
		mon = *Monitor_init();
	}
	pthread_mutex_lock(&mon.count_mutex);
	//if (CircularBuffer_IsFull(&mon.queue)) {
		//
	//}
	while(CircularBuffer_IsFull(&mon.queue)) {
		//sem_wait(&mon->full);
		pthread_cond_wait(&mon.full, &mon.count_mutex);		
	}
	CircularBuffer_Write(&mon.queue, alpha);
	CircularBuffer_PrintBuffer(&mon.queue);	
	if (CircularBuffer_OccupiedSpace(&mon.queue)<=1) {
		pthread_cond_signal(&mon.empty);	
	}
	pthread_mutex_unlock(&mon.count_mutex);
	
	//sem_wait(&mon->empty); 	
	//sem_wait(&mon->mutex);
}

char mon_remove(char replacementChar) {
	if (bMonInited==0) {
		printf("MONITOR NOT INITIALIZED");
	}
	pthread_mutex_lock(&mon.count_mutex);
	char value;
	int isFull = CircularBuffer_IsEmpty(&mon.queue);
	while (isFull) {
		//sem_wait(&mon->empty);
		pthread_cond_wait(&mon.empty, &mon.count_mutex);		
	}
	value = CircularBuffer_Read(&mon.queue, ' ');
	
	if (CircularBuffer_OccupiedSpace(&mon.queue)>= (&mon.queue)->size - 1  ) {
		pthread_cond_signal(&mon.empty);	
	}
	pthread_mutex_unlock(&mon.count_mutex);
	
	return value;
}