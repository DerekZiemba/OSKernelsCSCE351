
#include <stdio.h>
#include "../SharedResources.h"
#include <semaphore.h>

typedef struct cond_t {
	//Queue blockedThreads;
	int nWaitingThreads; 
	sem_t threadSemaphore;
	uint(*count)(struct cond_t *self);
} cond;

typedef struct Monitor_t {
	bool bIsInitialized;
	RingBuffer queue;
	cond full;
	cond empty;
	sem_t mutex;
	pthread_t *producers;
	pthread_t *consumers;
} Monitor;


void mon_insert(char alpha);
char mon_remove(char replacementChar);

Monitor *Monitor_init();

