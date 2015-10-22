#include "queue.h"

queue_t *Queue_init() {
	queue_t *queue = (queue_t *) calloc(sizeof(queue_t));
	return queue;
}

void Enqueue(queue_t *queue, void *data){
	node_t  *elem = (node_t *)calloc(sizeof(node_t));
	elem->data = data;
	elem->next = NULL;
    
	if (queue->head == NULL)
		queue->head = elem;
	else
		queue->tail->next = elem;
	
	queue->tail = elem;
	queue->size++;
}


void *Dequeue(queue_t *queue){
	void    *data = NULL;
    
	if (queue->size != 0){
		node_t *elem = queue->head;
		if (queue->size == 1)
			queue->tail = NULL;
		
		queue->head = queue->head->next;       
		queue->size--;
		
		data = elem->data;
		free(elem);
	}
        
	return data;
}

