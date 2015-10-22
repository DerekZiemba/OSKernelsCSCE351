#pragma once

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "../SharedResources.h"

typedef struct semaphore_t {
	
	int sem_blocking_id;
	int count;
	int threads_waiting;
} semaphore;

semaphore* sem_create(int initialCount);
void sem_signal(semaphore* sem);
void sem_signal(semaphore* sem);
void sem_wait(semaphore* sem);

#endif