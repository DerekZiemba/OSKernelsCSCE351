#pragma  once
#include <stdlib.h>
#include <stdio.h>
#include "../SharedResources.h"

#define BUFFER_SIZE 80
#define NUM_THREADS 8


typedef struct Cond {
	Queue blocked;
	bool condition;
} CV;

typedef struct Semaphore_t {
	jmp_buf context;
} Semaphore;


typedef struct Monitor_t {
	RingBuffer queue;
	CV full;
	CV empty;
	pthread_mutex_t mutex;
	pthread_t *producers;
	pthread_t *consumers;
} Monitor;


void mon_insert(char alpha);
char mon_remove(char replacementChar);
Monitor *Monitor_init();

int CV_count(CV* sem);
void CV_signal(CV* sem);
void CV_wait(CV* sem);

static void signal_handler(int sig) {
	switch (sig) {
	case SIGINT:
		
	}
}


