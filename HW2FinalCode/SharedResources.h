

#ifndef _SHAREDRESOURCES_h
#define _SHAREDRESOURCES_h

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define FALSE 0
#define TRUE  !0

typedef enum bool {
	false = FALSE,
	true  = TRUE,
} bool;


typedef struct RingBuffer RingBuffer;

#define READ_RINGBUFFER(type, buffer) (*(type *) ((buffer).elems + (buffer.tail) * sizeof(type)) 

	
/* Circular buffer object */
struct RingBuffer {
	int         size;   /* maximum number of elements           */
	int         tail;  /* index of oldest element              */
	int         head;    /* index at which to write new element  */
	char*		elems;  /* vector of elements                   */
};



/*The allocated size*/
static int RingBuffer_Size(RingBuffer *B){return (B->size - 1);}

/*The Number of allocated Elements*/
static int RingBuffer_Count(RingBuffer *B){return (B->head >= B->tail) ? (B->head - B->tail) : (B->head +  B->size - (B->tail));}

static bool RingBuffer_IsFull(RingBuffer *B) {return (RingBuffer_Size(B) - RingBuffer_Count(B) == 0) ? true : false;}

static bool RingBuffer_IsEmpty(RingBuffer *B) {return (RingBuffer_Count(B)) ? false : true;}

static void RingBuffer_Print(RingBuffer *B) {int i; for (i = 0; i < B->size; i++) printf("%c", B->elems[i]); printf("\n"); }


RingBuffer *RingBuffer_init(int size);
void RingBuffer_Write(RingBuffer *B, char elem);
char RingBuffer_Read(RingBuffer *B, char emptySymbol);


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