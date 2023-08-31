/* queue.h: Queue */

#ifndef QUEUE_H 
#define QUEUE_H

#include "thread.h"
#include <semaphore.h>

/* Structures */

typedef struct Queue Queue;
struct Queue {
    int   *data;
    int    sentinel;
    size_t capacity;

    size_t size;
    size_t reader;
    size_t writer;

    sem_t  lock;
    sem_t  consumed;
    sem_t  produced;
};

/* Functions */

Queue *	queue_create(int sentinel, size_t capacity);
void    queue_delete(Queue *q);

void	queue_push(Queue *q, int value);
int	queue_pop(Queue *q);

#endif

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
