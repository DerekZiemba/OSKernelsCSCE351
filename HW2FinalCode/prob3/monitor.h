#pragma  once
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../SharedResources.h"

#define BUFFER_SIZE 80
#define NUM_THREADS 8
#include "Semaphore.h"

typedef struct Monitor Monitor;

struct Monitor {
	RingBuff queue;
	CV full;
	CV empty;
	pthread_mutex_t mutex;
	pthread_t *producers;
	pthread_t *consumers;
};


void mon_insert(char alpha);
char mon_remove(char replacementChar);
Monitor *Monitor_init();


