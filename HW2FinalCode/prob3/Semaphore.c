#include "Semaphore.h"



CV* Semaphore_init(int initialCount) {
	CV* CV = calloc(1, sizeof(CV));
	CV->blocked = *Queue_init();
	CV->active = *Queue_init();
	CV->sem_blocking_id = 0;
	printf("CV created with initial count: %d\n", CV->count);
	return CV;
}

//// It performs the CV's signal operation.
//void Semaphore_signal(CV* sem) {
	////DISABLE_INTERRUPTS();
	//sem->count = sem->count + 1;
//
	//printf("Signal CV. Updated count: %d\n", sem->count);
	//
	//int blocked_threads = Semaphore_count(sem);
	//if (blocked_threads > 0) {//<= or >=
		//int i = 0;
		//for (i=0; i<=blocked_threads;i++) {
			//node_t blocked_node = *(node_t *) Dequeue(&sem->blocked);
			//
			//Enqueue(&sem->active, (void *) &blocked_node);			
		//}
		 //heads[sem->sem_blocking_id];
		//if (blocked_node != NULL) {
			//Node * new_node = (Node *) malloc(sizeof(Node));
			//new_node->thread = blocked_node->thread;
//
			//new_node->thread.scheduling_status = READY;
			//remove_node(blocked_node, sem->sem_blocking_id);
			//add_node(new_node, READY);
			//sem->threads_waiting = sem->threads_waiting - 1;
			//printf("Unblocked: %d\nThreads waiting: %d\n",blocked_node->thread.thread_id,sem->threads_waiting);
		//}
	//}
	////ENABLE_INTERRUPTS();
//}
//
//// It performs the CV's wait operation.
//void Semaphore_wait(CV* sem) {
////	DISABLE_INTERRUPTS();
	//sem->count = sem->count - 1;
//
	//printf("Wait on CV. Updated count: %d\n", sem->count);
	//if (sem->count < 0) {
		//running_thread[1]->thread.scheduling_status = sem->sem_blocking_id;
		//sem->threads_waiting = sem->threads_waiting + 1;
		//printf("Blocked: %d\nThreads waiting: %d\n",running_thread[1]->thread.thread_id,sem->threads_waiting);
//
	//}
	////ENABLE_INTERRUPTS();
	//// Wait for interrupt if blocked
	//int i = 0;
	//while (running_thread[1]->thread.scheduling_status == sem->sem_blocking_id)
		//for (i = 0; i < MAX; i++)
			//;
//}

int Semaphore_count(CV* sem) {
	return (sem->blocked).count;
}
