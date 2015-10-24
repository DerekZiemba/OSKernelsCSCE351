#ifndef _SHAREDRESOURCES_h
#define _SHAREDRESOURCES_h

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 80 //Recommend 80 if PRINT_BUFFER_ON_INSERT is true so that it fits in the console window
#define NUM_THREADS 8

#define PRINT_BUFFER_ON_INSERT true
#define PRINT_BUFFER_ON_REMOVE true
#define PRINT_ADDITIONAL_INFO false

/***************************************************************************
* Booleans
****************************************************************************/
#define FALSE 0
#define TRUE  !0
typedef enum bool { false = FALSE, true  = TRUE} bool;

/***************************************************************************
* Ring Buffer	
****************************************************************************/
typedef struct RingBuffer {
	uint		size;   /* Allocated number of elements           */
	int			tail;  /* index of oldest element              */
	int			head;    /* index at which to write new element  */
	char*		elems;  /* vector of elements                   */
	uint(*Count)(struct RingBuffer *self); /*The Number of allocated Elements*/
	bool(*IsFull)(struct RingBuffer *self);
	bool(*IsEmpty)(struct RingBuffer *self);
	char(*Read)(struct RingBuffer *self, char emptySymbol);
	void(*Write)(struct RingBuffer *self, char elem);	
	void(*Print)(struct RingBuffer *self);
}RingBuffer;

RingBuffer *RingBuffer_init(int size);

/***************************************************************************
* Linked List Queue
****************************************************************************/
typedef struct node_t {
	void            *data;
	struct node_t	*next;
	void(*Print)(struct node_t *self);
} node_t;

node_t *Node_init(void *data);

typedef struct Queue {
	uint		size;
	uint		count;
	node_t		*head;
	node_t		*tail;
	bool(*IsFull)(struct Queue *self);
	bool(*IsEmpty)(struct Queue *self);
	void(*Enqueue)(struct Queue *self, void* data);	
	void*(*Dequeue)(struct Queue *self);	
	void(*Print)(struct Queue *self);
} Queue;

Queue *Queue_init(uint maxsize);

/***************************************************************************
* Monitor
****************************************************************************/


/***************************************************************************
* Misc Functions
****************************************************************************/
char rand_alpha();
/*Delays a thread by creating a busy loop*/
void hardDelay(long multiplier);


#endif