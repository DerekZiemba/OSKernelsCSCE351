#pragma  once
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "Resources.h"

#define BUFFER_SIZE 100
#define NUM_THREADS 8

typedef struct Monitor Monitor;

struct Monitor {
	CircularBuffer queue;
	pthread_cond_t full;
	pthread_cond_t empty;
	pthread_mutex_t mutex;
	pthread_t *producers;
	pthread_t *consumers;
};

void mon_insert(char alpha);
char mon_remove(char replacementChar);
Monitor *Monitor_init();

