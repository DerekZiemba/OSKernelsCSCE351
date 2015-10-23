#include "monitor3.h"
#include "../SharedResources.h"

Monitor mon;
bool bMonInited = false;

CV* CV_init(int initialCount) {
	CV* CV = calloc(1, sizeof(CV));
	CV->blocked = *Queue_init();
	return CV;
}

inline static int CV_count(CV* sem) {	return (sem->blocked).count;}

void CV_signal(CV* sem);
void CV_wait(CV* sem);

static void signal_handler(int sig) {
	bool fullCond = (&mon.full)->condition;
	bool emptyCond = (&mon.empty)->condition;

	if(sig==SIGINT || sig == SIGALRM) {
		if (fullCond) {			
			Queue q = (&mon.full)->blocked;
			if(!Queue_IsEmpty(&q)) {				
				node_t *node = q.head;
				Semaphore *sem = (Semaphore*) node->data;
				jmp_buf *context = &sem->context;
				longjmp(*context, sig);					
			}
		}
		else if(emptyCond) {
			Queue q = (&mon.empty)->blocked;
			if (!Queue_IsEmpty(&q)) {				
				node_t *node = q.head;
				Semaphore *sem = (Semaphore*) node->data;
				jmp_buf *context = &sem->context;
				longjmp(*context, sig);					
			}
		}	
	}
	exit(sig);		
}


Monitor *Monitor_init(int timerMilliSecs) {
	bMonInited = true;	
	Monitor *B = calloc(1, sizeof(Monitor));	
	
	B->queue = *RingBuffer_init(BUFFER_SIZE);
	B->full = *CV_init(0);
	B->empty = *CV_init(0);
	pthread_mutex_init(&B->mutex, NULL);	
	B->producers = calloc(NUM_THREADS, sizeof(pthread_t));
	B->consumers = calloc(NUM_THREADS, sizeof(pthread_t));
	
	signal(SIGINT, signal_handler);
	signal(SIGALRM, signal_handler);
	ualarm(100*timerMilliSecs);
	while(true) {
		sleep(1);
	}
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

// It performs the CV's signal operation.
void CV_signal(CV* sem) {
	//DISABLE_INTERRUPTS();.
	sem->condition = true;	
	Queue q = &sem->blocked;
	if (!Queue_IsEmpty(&q)) {				
		node_t *node = q.head;
		while(node != NULL && node->data != NULL) {
			Semaphore *sem = (Semaphore*) node->data;			
			int sig= setjmp(&sem->context);
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


