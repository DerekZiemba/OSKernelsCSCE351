#ifndef _SHAREDRESOURCES_h
#define _SHAREDRESOURCES_h

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <pthread.h>


/***************************************************************************
* Booleans
****************************************************************************/
#define FALSE 0
#define TRUE  !0
typedef enum bool { false = FALSE, true  = TRUE} bool;

/***************************************************************************
* Ring Buffer	
****************************************************************************/
typedef struct RingBuffer RingBuffer;

struct RingBuffer {
	uint		size;   /* maximum number of elements           */
	int			tail;  /* index of oldest element              */
	int			head;    /* index at which to write new element  */
	char*		elems;  /* vector of elements                   */
};

/*The allocated size*/
static uint RingBuffer_Size(RingBuffer *B){return (B->size - 1);}
/*The Number of allocated Elements*/
static uint RingBuffer_Count(RingBuffer *B){return (B->head >= B->tail) ? (B->head - B->tail) : (B->head +  B->size - (B->tail));}
static bool RingBuffer_IsFull(RingBuffer *B) {return (RingBuffer_Size(B) - RingBuffer_Count(B) == 0) ? true : false;}
static bool RingBuffer_IsEmpty(RingBuffer *B) {return (RingBuffer_Count(B)) ? false : true;}
static void RingBuffer_Print(RingBuffer *B) {int i; for (i = 0; i < B->size; i++) printf("%c", B->elems[i]); printf("\n"); }

RingBuffer *RingBuffer_init(int size);
void RingBuffer_Write(RingBuffer *B, char elem);
char RingBuffer_Read(RingBuffer *B, char emptySymbol);


/***************************************************************************
* Linked List Queue
****************************************************************************/
typedef struct node_type {
	void                *data;
	struct node_type	*next;
} node_t;

typedef struct Queue Queue;
typedef struct Queue {
	uint		size;
	uint		count;
	node_t		*head;
	node_t		*tail;
} queue_t;

static void Node_Print(node_t *n) {
	if (n == NULL) return;
	if (n->data != NULL) {
		char c = (char*) n->data;
		printf("%c",c);
		Node_Print(n->next);
	}	
}

static void Queue_Print(Queue *q) {
	if (q->head != NULL) {
		node_t *elem = q->head;
		Node_Print(elem);
	}
	printf("\n");
}


/*The allocated Queue size*/
static uint Queue_Size(Queue *q){return (q->size);}
/*The Number of allocated Elements*/
static uint Queue_Count(Queue *q){return q->count;};

/*The queue is technically a linked list without a limit, but I'm limiting it by the size so it can replace ringbuffer and I can test it easier*/
static bool Queue_IsFull(Queue *q) {return (q->size - q->count == 0) ? true : false;}
static bool Queue_IsEmpty(Queue *q) {return (q->count) ? false : true;}

static bool Queue_GetNext(Queue *q, node_t *n) {
	if (n == NULL) return false;
	if (n->data != NULL) {
		char c = (char*)n->data;

		printf("%c", c);
		Node_Print(n->next);
	}
}


Queue *Queue_init();
void Enqueue(Queue *q, void *data);
void *Dequeue(Queue *q);

/***************************************************************************
* Monitor
****************************************************************************/


/***************************************************************************
* Misc Functions
****************************************************************************/
static char rand_alpha() {
	char charset[] = "abcdefghijklmnopqrstuvwxyz" "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static uint callcount; //So that each call will return a different letter
	callcount++;	
	srand((time(NULL) * callcount) - callcount);
	return charset[rand() %  (sizeof charset - 1)];
}

/*Delays a thread by creating a busy loop*/
static void hardDelay(long multiplier) {
	long i; for (i = 0; i < multiplier; i++) {
		long nops; for (nops = 0; nops < 50000; nops++) { asm("nop"); }
	}
}

#endif