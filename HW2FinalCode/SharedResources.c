#include "SharedResources.h"


uint RingBuffer_Count(RingBuffer *B){return (B->head >= B->tail) ? (B->head - B->tail) : (B->head +  B->size - (B->tail));}
bool RingBuffer_IsFull(RingBuffer *B) {return (B->size - B->Count(B) == 0) ? true : false;}
bool RingBuffer_IsEmpty(RingBuffer *B) {return B->Count ? false : true;}

/***************************************************************************
* Ring Buffer	
****************************************************************************/
/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking BIsFull(). */
void RingBuffer_Write(RingBuffer *B, char elem) {
	if (RingBuffer_IsFull(B)) {
		printf("ERROR BUFFER IS FULL CANNOT WRITE NOW\n");
	}
	else {
		B->elems[B->head] = elem;
		B->head = (B->head + 1) % (B->size -1);
		if (B->head == B->tail)
			B->tail = (B->tail + 1) % (B->size -1); /* full, overwrite */
	}
}
 
/* Read oldest element. App must ensure !BIsEmpty() first. */
char RingBuffer_Read(RingBuffer *B, char emptySymbol) {
	char elem = B->elems[B->tail];	
	B->elems[B->tail] = emptySymbol;
	B->tail = (B->tail + 1) % (B->size - 1);	
	return elem;
}

void RingBuffer_Print(RingBuffer *B) {
	int i; 
	for (i = 0; i < B->size; i++) 
		printf("%c", B->elems[i]); 
	printf("\n");
}

RingBuffer *RingBuffer_init(int size) {
	RingBuffer *B = calloc(1, sizeof(RingBuffer));	
	B->size  = size; /* include empty elem */
	B->tail = 0;
	B->head   = 0;
	B->elems = calloc((B->size), sizeof(char));	
	B->Count = RingBuffer_Count;
	B->IsFull = RingBuffer_IsFull;
	B->IsEmpty = RingBuffer_IsEmpty;
	B->Read = RingBuffer_Read;
	B->Write = RingBuffer_Write;
	B->Print = RingBuffer_Print;
	return B;
}



/***************************************************************************
* Linked List Queue
****************************************************************************/
/*The queue is technically a linked list without a limit, 
* but I'm limiting it by the size so it can replace ringbuffer and I can test it easier
* Passing in 0 will make it infinit*/
bool Queue_IsFull(Queue *q) {return (q->size - q->count == 0) ? true : false;}
bool Queue_IsEmpty(Queue *q) {return (q->count) ? false : true;}

void Enqueue(Queue *q, void *data) {
	if (!q->size == 0 || q->count >= q->size) {
		printf("ERROR QUEUE IS FULL CANNOT ADD NOW\n");
		return;
	}
	node_t  *elem = Node_init(data);
	if (q->head == NULL) q->head = elem;
	else q->tail->next = elem;	
	q->tail = elem;
	q->count++;
}


void *Dequeue(Queue *q) {
	void *data = NULL;   
	if (q->count != 0) {
		node_t *elem = q->head;
		if (q->count == 1) q->tail = NULL;		
		q->head = q->head->next;       
		q->count--;		
		data = elem->data;
		free(elem);
	}       
	return data;
}

//void Queue_Print(Queue *q) {
	//if (q->head != NULL) {
		//node_t *elem = q->head;
		//elem->Print(elem);
	//}
	//printf("\n");
//}
//
//void Node_Print(node_t *n) {
	//if (n == NULL) return;
	//if (n->data != NULL) {
		//char c = (char*) n->data;
		//printf("%c", c);
		//Node_Print(n->next);
	//}	
//}

node_t *Node_init(void *data) {
	node_t  *elem = (node_t *)calloc(1, sizeof(node_t));
	elem->data = data;
	elem->next = NULL;   
	//elem->Print = Node_Print;
	return elem;
}

Queue *Queue_init(uint maxsize) {
	Queue *q = (Queue *) calloc(1, sizeof(Queue));
	q->size = maxsize;
	q->count = 0;
	q->IsFull = Queue_IsFull;
	q->IsEmpty = Queue_IsEmpty;
	q->Enqueue = Enqueue;
	q->Dequeue = Dequeue;
	//q->Print = Queue_Print;
	return q;
}


/***************************************************************************
* Misc Functions
****************************************************************************/
char rand_alpha() {
	char charset[] = "abcdefghijklmnopqrstuvwxyz" "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static uint callcount; //So that each call will return a different letter
	callcount++;	
	srand((time(NULL) * callcount) - callcount);
	int r = rand() - callcount;
	return charset[r %  (sizeof charset - 1)];
}

void hardDelay(long multiplier) {
	long i; for (i = 0; i < multiplier; i++) {
		long nops; for (nops = 0; nops < 50000; nops++) { asm("nop"); }
	}
}