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
* Generic Linked List Queue
****************************************************************************/
node_t *Node_init(void *data) {
	node_t  *elem = calloc(1, sizeof(node_t));
	elem->data = data;
	elem->parentNode = NULL;
	elem->childNode = NULL;
	return elem;
}

/*This was just an experiment.
 *  Wanted it to work for all and be used in the likes of SearchChildNodesByThreadID
 *  but I couldn't get things to cast back correctly.
 *   Also the GCC version used by altera doesn't allow trampolines which was essential for this.
 *   trampolines = functions inside of functions like javascript.  Just found out what those are a few minutes ago */
void Node_Iterator(node_t *n, bool(*callback)(node_t* data, void* context), void *context) {
	if (n == NULL) return;
	if (n->data != NULL) {
		if (callback(n, context)) {
			Node_Iterator(n->childNode, callback, context);
		}
	}
}



/*The queue is technically a linked list without a limit,
* but I'm limiting it by the size so it can replace ringbuffer and I can test it easier
* Passing in 0 will make it infinit*/
Queue *Queue_init(uint32_t maxsize) {
	Queue *q = calloc(1, sizeof(Queue));
	q->maxsize = maxsize;
	q->count = 0;
	return q;
}


void Queue_Enqueue(Queue *q, void *data) {
	if (q->maxsize == 0) {
		//This is a limitless Queue
	}
	else if(q->count < 0 || q->maxsize < 0) {
		printf("ERROR: Invalid Pointer To Queue\n");
		return;
	}
	else if (q->count >= q->maxsize) {
		printf("ERROR: Queue is Full\n");
		return;
	}
	node_t  *elem = Node_init(data);
	if (q->count == 0) {
		q->firstNode = elem;
		q->lastNode = q->firstNode;
	}
	else {
		elem->parentNode = q->lastNode;
		q->lastNode->childNode = elem;
	}
	q->lastNode = elem;
	q->count++;
}
		
void* Queue_Dequeue(Queue *q) {
	if (q->count < 0 || q->maxsize < 0) {
		printf("ERROR: Invalid Pointer To Queue\n");
		return NULL;
	}
	
	void *data = NULL;
	if (q->count != 0) {
		node_t *oldLeadingNode = q->firstNode;
		data = oldLeadingNode->data;

		q->firstNode = oldLeadingNode->childNode;
		q->count--;


		if (q->count > 0) {
			free(q->firstNode->parentNode);
			q->firstNode->parentNode = NULL;
		}
		else if (q->count == 0) {
			free(q->firstNode);
			free(q->lastNode);
			q->firstNode = NULL;
			q->lastNode = NULL;
			oldLeadingNode = NULL;
		}
		else {
			q->firstNode = NULL;
			q->lastNode = NULL;
			oldLeadingNode = NULL;
		}
	}
	else {
		printf("ERROR: Nothing to Dequeue\n");
	}
	return data;
}

//void* Queue_Dequeue(Queue *q) {
	//if (q->count < 0 || q->maxsize < 0) {
		//printf("ERROR: Invalid Pointer To Queue\n");
		//return NULL;
	//}
	//
	//void *data = NULL;
	//if (q->count != 0) {
		//node_t *oldLeadingNode = q->firstNode;
		//data = oldLeadingNode->data;
//
		//if (q->count == 1) {
			//free(q->lastNode);
		//}
//
		//q->firstNode = oldLeadingNode->childNode;
		//q->count--;
//
		//if (q->count > 0) {
			//q->firstNode->parentNode = NULL;
			//free(oldLeadingNode);
		//}
		//else {
			////Calling free when there's no more nodes will throw a SIGABRT.  No idea why.
			////free (q->firstNode);
			////free(q->lastNode);
			////free(oldLeadingNode);
			//q->firstNode = NULL;
			//q->lastNode = NULL;
			//oldLeadingNode = NULL;
		//}
	//}
	//else {
		//printf("ERROR: Nothing to Dequeue\n");
	//}
	//return data;
//}

void* Queue_Pull(Queue *q) {
	if (q->count < 0 || q->maxsize < 0) {
		printf("ERROR: Invalid Pointer To Queue\n");
		return NULL;
	}
	
	void *data = NULL;
	if (q->count != 0) {
		node_t *oldLeadingNode = q->firstNode;
		data = oldLeadingNode->data;

		if (q->count == 1) {
			free(q->lastNode);
		}

		q->firstNode = oldLeadingNode->childNode;
		q->count--;

		if (q->count > 0) {
			q->firstNode->parentNode = NULL;
			free(oldLeadingNode);
		}
		else {
			//Calling free when there's no more nodes will throw a SIGABRT.  No idea why.
			//free (q->firstNode);
			//free(q->lastNode);
			//free(oldLeadingNode);
			q->firstNode = NULL;
			q->lastNode = NULL;
			oldLeadingNode = NULL;
		}
	}
	else {
		printf("ERROR: Nothing to Dequeue\n");
	}
	return data;
}

void* Queue_Peek(Queue *q) {
	void *data = NULL;
	if (q->count != 0) {
		data = q->firstNode->data;
	}
	return data;
}


bool Queue_IsFull(Queue *q) {return (q->maxsize - q->count == 0) ? true : false;}
bool Queue_IsEmpty(Queue *q) {return (q->count) ? false : true;}

void Queue_Print(Queue *q, char * format) {
	bool lambda(node_t* data, void *context) {
		printf((char*) context, (char*)data->data);
		return true;
	}
	Node_Iterator(q->firstNode, &lambda, (void*) format);
	printf("\n");
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