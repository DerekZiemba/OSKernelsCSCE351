
#include "SharedResources.h"


pno Initializer(RingBuff *B,int size) {return RingBuff_init(size);}
//pno Initializer(RingBuff *B, int size) {return RingBuff_init(B, size);}

RingBuff *RingBuff_init(int size) {
	RingBuff *B = calloc(1, sizeof(RingBuff));	
	B->size  = size + 1; /* include empty elem */
	B->tail = 0;
	B->head   = 0;
	B->elems = calloc(B->size, sizeof(char));	
	return B;
}


int RingBuff_MaxSize(RingBuff *B) {
	return B->size - 1 ;	
}

int RingBuff_IsFull(RingBuff *B) {
	return RingBuff_MaxSize(B)  - RingBuff_OccupiedSpace(B) == 0;
}

int RingBuff_IsEmpty(RingBuff *V){
	return RingBuff_OccupiedSpace(V) == 0;	
}

int RingBuff_OccupiedSpace(RingBuff *B) {
	if (B->head >= B->tail) {
		return B->head - B->tail;
	}
	else {
		return B->head +  B->size - (B->tail);
	}	
}

int RingBuff_FreeSpace(RingBuff *B) {
	return ((B)->size - RingBuff_OccupiedSpace((B)));
}

/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking BIsFull(). */
void RingBuff_Write(RingBuff *B, char elem) {
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
char RingBuff_Read(RingBuff *B, char emptySymbol) {
	char elem = B->elems[B->tail];	
	B->elems[B->tail] = emptySymbol;
	B->tail = (B->tail + 1) % B->size;	
	return elem;
}

void RingBuff_PrintBuffer(RingBuff *B) {
	int size = B->size;
	int i;
	for (i=0; i< size; i++)
		printf("%c", B->elems[i]);
	printf("\n");
}

char rand_alpha() {
	char charset[] = "abcdefghijklmnopqrstuvwxyz"
					 "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static uint callcount; //So that each call will return a different letter
	callcount++;	
	time_t t = time(NULL);
	ulong seed = (t * callcount) - callcount;
	srand(seed);
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
