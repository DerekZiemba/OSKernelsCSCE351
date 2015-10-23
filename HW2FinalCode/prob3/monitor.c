#include "monitor.h"
#include "../SharedResources.h"

CV* CV_init(int initialCount) {
	CV* CV = calloc(1, sizeof(CV));
	CV->blocked = *Queue_init();
	return CV;
}

Monitor mon;
bool bMonInited = false;

Monitor *Monitor_init() {
	bMonInited = true;	
	Monitor *B = calloc(1, sizeof(Monitor));	
	
	B->queue = *RingBuffer_init(BUFFER_SIZE);
	B->full = *CV_init(0);
	B->empty = *CV_init(0);
	pthread_mutex_init(&B->mutex, NULL);	
	B->producers = calloc(NUM_THREADS, sizeof(pthread_t));
	B->consumers = calloc(NUM_THREADS, sizeof(pthread_t));
	
	return B;
}


void mon_insert(char alpha) {
	if (!bMonInited) 
		mon = *Monitor_init();
	
	pthread_mutex_lock(&mon.mutex);
		
	bool isFull = RingBuffer_IsFull(&mon.queue);
	if(isFull) {
		//Broadcast to threads that are currently suspended and waiting on the full condition they can start
		pthread_cond_broadcast(&mon.full); 
		//Suspend this thread until the threads that just started broadcast the empty condition. 
		pthread_cond_wait(&mon.empty, &mon.mutex);	
	}
	
	RingBuffer_Write(&mon.queue, alpha);
	RingBuffer_Print(&mon.queue);	
	
	//Enqueue(&mon.queue, (void*)alpha);
	//Queue_Print(&mon.queue);		
	pthread_mutex_unlock(&mon.mutex);	
}
	
char mon_remove(char replacementChar) {
	if (!bMonInited) 
		printf("MONITOR NOT INITIALIZED");
	
	pthread_mutex_lock(&mon.mutex);
	
	bool isEmpty = RingBuffer_IsEmpty(&mon.queue);
	if (isEmpty) {		
		pthread_cond_broadcast(&mon.empty); //Tell the threads that are waiting on empty condition to start
		pthread_cond_wait(&mon.full, &mon.mutex);//Wait for those threads to broadcast the full condition.	
	}

	//char value = (char*)Dequeue(&mon.queue);
	char value = RingBuffer_Read(&mon.queue, ' ');
	pthread_mutex_unlock(&mon.mutex);
	return value;
}



jmp_buf env;



int CV_count(CV* sem) {	return (sem->blocked).count;}

// It performs the CV's signal operation.
void CV_signal(CV* sem) {
	//DISABLE_INTERRUPTS();
	int blocked_threads = CV_count(sem);
	if (blocked_threads > 0) {//<= or >=
		int i = 0;
		for (i = 0; i <= blocked_threads; i++) {
			node_t blocked_node = *(node_t *) Dequeue(&sem->blocked);
			Semaphore context = (Semaphore *) blocked_node.data;	
			
		}
		heads[sem->sem_blocking_id];
		if (blocked_node != NULL) {
			Node * new_node = (Node *) malloc(sizeof(Node));
			new_node->thread = blocked_node->thread;

			new_node->thread.scheduling_status = READY;
			remove_node(blocked_node, sem->sem_blocking_id);
			add_node(new_node, READY);
			sem->threads_waiting = sem->threads_waiting - 1;
			printf("Unblocked: %d\nThreads waiting: %d\n", blocked_node->thread.thread_id, sem->threads_waiting);
		}
	}
}
//
// It performs the CV's wait operation.
void CV_wait(CV* sem) {
//	DISABLE_INTERRUPTS();
	sem->count = sem->count - 1;

	printf("Wait on CV. Updated count: %d\n", sem->count);
	if (sem->count < 0) {
		running_thread[1]->thread.scheduling_status = sem->sem_blocking_id;
		sem->threads_waiting = sem->threads_waiting + 1;
		printf("Blocked: %d\nThreads waiting: %d\n", running_thread[1]->thread.thread_id, sem->threads_waiting);

	}
	//ENABLE_INTERRUPTS();
	// Wait for interrupt if blocked
	int i = 0;
	while (running_thread[1]->thread.scheduling_status == sem->sem_blocking_id)
		for (i = 0; i < MAX; i++)
			;
}


