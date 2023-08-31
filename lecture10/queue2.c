/* queue2.c: Queue (with Condition Variable)
 *  
 *  Problem: Signal either producer or consumer when consumer should signal
 *  producer and consumer should signal producer.
 **/

#include "queue.h"

Queue *	queue_create(int sentinel, size_t capacity) {
    Queue *q = calloc(1, sizeof(Queue));
    if (q) {
    	q->data     = calloc(capacity, sizeof(int));
    	q->sentinel = sentinel;
    	q->capacity = capacity;

    	mutex_init(&q->lock, NULL);     
    	cond_init(&q->cond, NULL);      // Initialize condition variable
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
    mutex_lock(&q->lock);              

    while (q->size >= q->capacity) {
        cond_wait(&q->cond, &q->lock);  // Yield and wait until woken up
    }

    q->data[q->writer] = value;
    q->writer	       = (q->writer + 1) % q->capacity;
    q->size++;

    cond_signal(&q->cond);              // Wake up another thread
    mutex_unlock(&q->lock);           
}

int	queue_pop(Queue *q) {
    mutex_lock(&q->lock);            

    while (q->size == 0) {
        cond_wait(&q->cond, &q->lock);  // Yield and wait until woken up
    }

    int value = q->data[q->reader];
    if (value != q->sentinel) {
        q->reader = (q->reader + 1) % q->capacity;
        q->size--;
    }

    cond_signal(&q->cond);              // Wake up another thread
    mutex_unlock(&q->lock);         
    return value;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
