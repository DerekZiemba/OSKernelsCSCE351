#ifndef _SHAREDRESOURCES_h
#define _SHAREDRESOURCES_h

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 12 //Recommend 80 if PRINT_BUFFER_ON_INSERT is true so that it fits in the console window
#define NUM_THREADS 12

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
* Interchangeability between altera and native types
****************************************************************************/

//#define u32 alt_u32
//#define i32 alt_32
//#define u16 alt_u16
//#define i16 alt_16

#define uint32 uint32_t
#define int32 int32_t
#define uint16 uint16_t
#define int16 int16_t



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
* Generic Linked List Queue
****************************************************************************/
typedef struct node_t {
	void*			data;
	struct node_t*	parentNode;
	struct node_t*	childNode;
} node_t;

node_t*	Node_init(void *data);
void 	Node_Iterator(node_t *n, bool(*callback)(node_t* data, void* context), void *context);

typedef struct Queue {
	int32	maxsize;
	int32	count;
	node_t*	firstNode;
	node_t*	lastNode;
} Queue;

//Pass In 0 for unrestrained queue.
Queue*	Queue_init(uint32 max_size);
void	Queue_Enqueue(Queue *q, void *data);
void*	Queue_Dequeue(Queue *q);
void*	Queue_Peek(Queue *q);
bool 	Queue_IsFull(Queue *q);
bool 	Queue_IsEmpty(Queue *q);
void 	Queue_Print(Queue *q, char * format);


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