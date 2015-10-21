#pragma  once
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "Resources.h"

#define BUFFER_SIZE 100
#define NUM_THREADS 8

typedef struct Monitor Monitor;

struct Monitor {
	CircularBuffer queue;
	pthread_cond_t full;
	pthread_cond_t empty;
	sem_t mutex;
	pthread_mutex_t count_mutex;
	pthread_cond_t count_threshold_cv;
	pthread_t *producers;
	pthread_t *consumers;
	long taskids;
	int count;
};

void mon_insert(char alpha);
char mon_remove(char replacementChar);
Monitor *Monitor_init();

