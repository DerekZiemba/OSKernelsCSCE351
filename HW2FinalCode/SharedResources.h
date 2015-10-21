

#ifndef _SHAREDRESOURCES_h
#define _SHAREDRESOURCES_h

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


typedef struct RingBuff RingBuff;

/* Circular buffer object */
struct RingBuff {
	int         size;   /* maximum number of elements           */
	int         tail;  /* index of oldest element              */
	int         head;    /* index at which to write new element  */
	char*		elems;  /* vector of elements                   */
};


RingBuff *RingBuff_init(int size);

int RingBuff_MaxSize(RingBuff *B);
int RingBuff_IsFull(RingBuff *B);
int RingBuff_IsEmpty(RingBuff *B);
int RingBuff_OccupiedSpace(RingBuff *B);
int RingBuff_FreeSpace(RingBuff *B);

void RingBuff_Write(RingBuff *B, char elem);
char RingBuff_Read(RingBuff *B,char emptySymbol);

void RingBuff_PrintBuffer(RingBuff *B);


char rand_alpha();
void hardDelay(long multiplier);

#endif