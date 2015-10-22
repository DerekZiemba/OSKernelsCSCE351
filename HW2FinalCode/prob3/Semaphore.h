#pragma once

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "../SharedResources.h"
#include "queue.h"

typedef struct CV_t {
	queue_t queue;
	queue_t blocked;
	queue_t active;
	
	int sem_blocking_id;
	int count;
	int threads_waiting;
} CV;

CV* Semaphore_init(int initialCount);
int Semaphore_count(CV* sem);
void Semaphore_signal(CV* sem);
void Semaphore_wait(CV* sem);

#endif
