//#include "Semaphore.h"
//
//
//
//semaphore* sem_create(int initialCount) {
	//semaphore* semaphore = (semaphore*) malloc(36);
	//semaphore->count = initialCount;
	//semaphore->threads_waiting = 0;
	//semaphore->sem_blocking_id = 0;
	//printf("Semaphore created with initial count: %d\n", semaphore->count);
	//return semaphore;
//}
//
//// It performs the semaphore's signal operation.
//void sem_signal(semaphore* sem) {
	////DISABLE_INTERRUPTS();
	//sem->count = sem->count + 1;
//
	//printf("Signal semaphore. Updated count: %d\n", sem->count);
	//
	//if (sem->count <= 0) {//<= or >=
		//Node *blocked_node = heads[sem->sem_blocking_id];
		//if (blocked_node != NULL) {
			//Node * new_node = (Node *) malloc(sizeof(Node));
			//new_node->thread = blocked_node->thread;
//
			//new_node->thread.scheduling_status = READY;
			//remove_node(blocked_node, sem->sem_blocking_id);
			//add_node(new_node, READY);
			//sem->threads_waiting = sem->threads_waiting - 1;
			//printf("Unblocked: %d\nThreads waiting: %d\n",
				//blocked_node->thread.thread_id,
				//sem->threads_waiting);
		//}
	//}
	////ENABLE_INTERRUPTS();
//}
//
//// It performs the semaphore's wait operation.
//void sem_wait(semaphore* sem) {
////	DISABLE_INTERRUPTS();
	//sem->count = sem->count - 1;
//
	//printf("Wait on semaphore. Updated count: %d\n", sem->count);
	//if (sem->count < 0) {
		//running_thread[1]->thread.scheduling_status = sem->sem_blocking_id;
		//sem->threads_waiting = sem->threads_waiting + 1;
		//printf("Blocked: %d\nThreads waiting: %d\n",
			//running_thread[1]->thread.thread_id,
			//sem->threads_waiting);
//
	//}
	////ENABLE_INTERRUPTS();
	//// Wait for interrupt if blocked
	//int i = 0;
	//while (running_thread[1]->thread.scheduling_status == sem->sem_blocking_id)
		//for (i = 0; i < MAX; i++)
			//;
//}
