#include "SharedResources.h"


RingBuffer *RingBuffer_init(int size) {
	RingBuffer *B = calloc(1, sizeof(RingBuffer));	
	B->size  = size + 1; /* include empty elem */
	B->tail = 0;
	B->head   = 0;
	B->elems = calloc(B->size, sizeof(char));	
	return B;
}


/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking BIsFull(). */
void RingBuffer_Write(RingBuffer *B, char elem) {
	if (RingBuffer_IsFull(B)) {
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
char RingBuffer_Read(RingBuffer *B, char emptySymbol) {
	char elem = B->elems[B->tail];	
	B->elems[B->tail] = emptySymbol;
	B->tail = (B->tail + 1) % B->size;	
	return elem;
}
