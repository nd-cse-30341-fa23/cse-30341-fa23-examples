/* queue0.c: Queue 
 *  
 *  Problem: Lack of locks lead to race conditions (deadlock, inconsistent output)
 **/

#include "queue.h"

Queue *	queue_create(int sentinel, size_t capacity) {
    Queue *q = calloc(1, sizeof(Queue));
    if (q) {
    	q->data     = calloc(capacity, sizeof(int));
    	q->sentinel = sentinel;
    	q->capacity = capacity;
    }
    return q;
}

void	queue_delete(Queue *q) {
    if (q) {
    	free(q->data);
    	free(q);
    }
}

void	queue_push(Queue *q, int value) {
    while (q->size >= q->capacity) {
    	// Spin
    }

    q->data[q->writer] = value;
    q->writer	       = (q->writer + 1) % q->capacity;
    q->size++;
}

int	queue_pop(Queue *q) {
    while (q->size == 0) {
    	// Spin
    }

    int value = q->data[q->reader];
    if (value != q->sentinel) {
        q->reader = (q->reader + 1) % q->capacity;
        q->size--;
    }
    return value;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
