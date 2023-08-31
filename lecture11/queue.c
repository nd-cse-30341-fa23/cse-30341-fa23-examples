/* queue.c: Queue (Semaphore) */

#include "queue.h"

Queue *	queue_create(int sentinel, size_t capacity) {
    Queue *q = calloc(1, sizeof(Queue));
    if (q) {
    	q->data     = calloc(capacity, sizeof(int));
    	q->sentinel = sentinel;
    	q->capacity = capacity;

        // Initialize mutex
        sem_init(&q->lock    , 0, 1);

        // Initialize condition variable
        sem_init(&q->produced, 0, 0);
        sem_init(&q->consumed, 0, q->capacity);
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
    sem_wait(&q->consumed);     // Decrement and wait if q->consumed < 0
    sem_wait(&q->lock);         // Decrement and wait if q->lock     < 0

    q->data[q->writer] = value;
    q->writer	       = (q->writer + 1) % q->capacity;
    q->size++;

    sem_post(&q->lock);         // Increment q->lock     and wake up a thread
    sem_post(&q->produced);     // Increment q->produced and wake up a thread
}

int	queue_pop(Queue *q) {
    sem_wait(&q->produced);     // Decrement and wait if q->produced < 0
    sem_wait(&q->lock);         // Decrement and wait if q->lock     < 0
    int value = q->data[q->reader];
    if (value != q->sentinel) {
        q->reader = (q->reader + 1) % q->capacity;
        q->size--;
    }
    sem_post(&q->lock);         // Increment q->lock     and wake up a thread
    if (value != q->sentinel) {
        sem_post(&q->consumed); // Increment q->consumed and wake up a thread
    } else {
        sem_post(&q->produced); // Increment q->produced and wake up a thread
    }
    return value;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
