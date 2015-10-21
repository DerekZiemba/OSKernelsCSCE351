/*
 *  Created on: Jan 22, 2014
 *      Author: Derek Ziemba
 *  NOTE: Heavily modified from version I created for Arduino in CSCE222. 
 */

#include "Resources.h"
#include <stdlib.h>
#include <stdio.h>

CircularBuffer *CircularBuffer_init(int size) {
	CircularBuffer *B = calloc(1, sizeof(CircularBuffer));	
	B->size  = size + 1; /* include empty elem */
	B->tail = 0;
	B->head   = 0;
	B->elems = calloc(B->size, sizeof(char));	
	return B;
}


int CircularBuffer_IsFull(CircularBuffer *cb) {
	return cb->size  - CircularBuffer_OccupiedSpace(cb) -1 == 0;
}

int CircularBuffer_IsEmpty(CircularBuffer *cb){
	return CircularBuffer_OccupiedSpace(cb) == 0;	
}

int CircularBuffer_OccupiedSpace(CircularBuffer *B) {
	if (B->head >= B->tail) {
		return B->head - B->tail;
	}
	else {
		return B->head +  B->size - (B->tail);
	}	
}

/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking BIsFull(). */
void CircularBuffer_Write(CircularBuffer *B, char elem) {
	if (CircularBuffer_IsFull(B)) {
		printf("ERROR BUFFER IS FULL CANNOT WRITE NOW\n");
	}
	else {
		B->elems[B->head] = elem;
		B->head = (B->head + 1) % B->size;
		if (B->head == B->tail)
			B->tail = (B->tail + 1) % B->size; /* full, overwrite */
	}
}
 
/* Read oldest element. App must ensure !BIsEmpty() first. */
char CircularBuffer_Read(CircularBuffer *B, char emptySymbol) {
	char elem = B->elems[B->tail];	
	B->elems[B->tail] = emptySymbol;
	B->tail = (B->tail + 1) % B->size;	
	return elem;
}

void CircularBuffer_PrintBuffer(CircularBuffer *B) {
	int size = B->size;
	int i;
	for (i=0; i< size; i++)
		printf("%c", B->elems[i]);
	printf("\n");
}

char rand_char() {
	char charset[] =  "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	return charset[rand() %  (sizeof charset - 1)];
}

void hardDelay(long multiplier) {
	//usleep(1000*multiplier);
	long i = 0;
	for (i = 0; i < multiplier; i++) {
		long nops = 0;
		for (nops = 0; nops < 50000; nops++) {asm("nop"); }
	}
}
