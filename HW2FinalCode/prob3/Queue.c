#include "../SharedResources.h"

/*Pass 0 for unrestrained queue*/
Queue *Queue_init(uint maxsize) {
	Queue *q = (Queue *) calloc(1, sizeof(queue_t));
	q->size = maxsize;
	q->count = 0;
	return q;
}

void Enqueue(Queue *q, void *data) {
	if (!q->size==0 || q->count >= q->size) {
		printf("ERROR QUEUE IS FULL CANNOT ADD NOW\n");
		return;
	}
	node_t  *elem = (node_t *)calloc(1,sizeof(node_t));
	elem->data = data;
	elem->next = NULL;
    
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

void Queue_Print(Queue *q) {
	if (q->head != NULL) {
		node_t *elem = q->head;
		Node_Print(elem);
	}
	printf("\n");
}

void Node_Print(node_t *n) {
	if (n == NULL) return;
	if (n->data != NULL) {
		char c = (char*) n->data;
		printf("%c", c);
		Node_Print(n->next);
	}	
}




//

