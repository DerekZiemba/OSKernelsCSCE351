
#include "SharedResources.h"

Buffer *RingBuff_init(int size) {
	Buffer *B = calloc(1, sizeof(Buffer));	
	B->size  = size + 1; /* include empty elem */
	B->tail = 0;
	B->head   = 0;
	B->elems = calloc(B->size, sizeof(char));	
	return B;
}

/*The allocated size*/
int Buff_Size(Buffer *B) {	return B->size - 1 ;	}

int Buff_Count(Buffer *B) {
	if (B->head >= B->tail)  return B->head - B->tail;
	else return B->head +  B->size - (B->tail);	
}

int RingBuff_IsFull(Buffer *B) {
	return Buff_Size(B)  - Buff_Count(B) == 0;
}

int RingBuff_IsEmpty(Buffer *V){
	return Buff_Count(V) == 0;	
}



int RingBuff_FreeSpace(Buffer *B) {
	return ((B)->size - Buff_Count((B)));
}

/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking BIsFull(). */
void RingBuff_Write(Buffer *B, char elem) {
	if (RingBuff_IsFull(B)) {
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
char RingBuff_Read(Buffer *B, char emptySymbol) {
	char elem = B->elems[B->tail];	
	B->elems[B->tail] = emptySymbol;
	B->tail = (B->tail + 1) % B->size;	
	return elem;
}

void RingBuff_PrintBuffer(Buffer *B) {
	int size = B->size;
	int i;
	for (i=0; i< size; i++)
		printf("%c", B->elems[i]);
	printf("\n");
}

