#pragma once

#ifndef  _QUEUE_H_
#define _QUEUE_H_


#include <stdio.h>
#include <stdlib.h>


typedef struct node_type{
	void                *data;
	struct node_type	*next;
} node_t;

typedef struct queue_type{
	node_t              *head;
	node_t              *tail;
	unsigned int        size;
} queue_t;

queue_t *Queue_init();
void Enqueue(queue_t *queue, void *data);
void *Dequeue(queue_t *queue);

#endif // ! QUEUE_H_