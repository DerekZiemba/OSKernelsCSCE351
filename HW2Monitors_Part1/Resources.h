/*
*  Created on: Jan 22, 2014
*      Author: Derek Ziemba
*  NOTE: Heavily modified from version I created for Arduino in CSCE222. 
*/

#ifndef _RESOURCES_h
#define _RESOURCES_h

typedef struct CircularBuffer CircularBuffer;

/* Circular buffer object */
struct CircularBuffer {
	int         size;   /* maximum number of elements           */
	int         tail;  /* index of oldest element              */
	int         head;    /* index at which to write new element  */
	char*		elems;  /* vector of elements                   */
};


CircularBuffer *CircularBuffer_init(int size);

int CircularBuffer_IsFull(CircularBuffer *cb);
int CircularBuffer_IsEmpty(CircularBuffer *cb);
int CircularBuffer_OccupiedSpace(CircularBuffer *cb);
void CircularBuffer_Write(CircularBuffer *cb, char elem);
char CircularBuffer_Read(CircularBuffer *cb,char emptySymbol);
void CircularBuffer_PrintBuffer(CircularBuffer *cb);


//#define CircularBuffer_IsFull(B) (CircularBuffer_OccupiedSpace((B)) - (B)->size-1 == 0)
//#define CircularBuffer_IsEmpty(B) (CircularBuffer_OccupiedSpace((B)) == 0)
#define CircularBuffer_FreeSpace(B) ((B)->size - CircularBuffer_OccupiedSpace((B)))

char rand_char(void);
void hardDelay(long multiplier);

#endif