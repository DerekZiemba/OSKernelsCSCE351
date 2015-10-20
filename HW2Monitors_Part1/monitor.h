#pragma  once
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "Resources.h"

#define BUFFER_SIZE 100

typedef struct Monitor Monitor;

struct Monitor {
	CircularBuffer queue;
	sem_t full;
	sem_t empty;
	sem_t mutex;
	pthread_t producers[];
	pthread_t consumers[];
	int count;
};

void mon_insert(char alpha);
char mon_remove(char replacementChar);
Monitor *Monitor_init();

