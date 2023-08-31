/* pc.c: Producer/Consumer */

#include "queue.h"
#include "thread.h"

/* Constants */

const size_t NITEMS	= 1<<10;
const size_t NTHREADS	= 8;
const int    SENTINEL   = -1;
const int    CAPACITY	= NTHREADS*NTHREADS;

/* Threads */

void *	producer(void *arg) {
    Queue *q = (Queue *)arg;

    for (size_t i = 0; i < NITEMS; i++) {
    	queue_push(q, i);	// Add item to queue
    }

    queue_push(q, q->sentinel);	// Shut it down
    return 0;
}

void *	consumer(void *arg) {
    Queue *q     = (Queue *)arg;
    size_t total = 0;
    int    value = 0;

    while (value != q->sentinel) {
    	total += value;
    	value  = queue_pop(q);
    	printf("total = %lu, value = %d\n", total, value);
    }

    return (void *)total;
}

/* Main execution */

int main(int argc, char *argv[]) {
    Queue *q = queue_create(SENTINEL, CAPACITY);
    Thread t[NTHREADS];

    thread_create(&t[0], NULL, producer, q);
    for (size_t i = 1; i < NTHREADS; i++) {
    	thread_create(&t[i], NULL, consumer, q);
    }

    size_t result, total = 0;
    for (size_t i = 0; i < NTHREADS; i++) {
    	thread_join(t[i], (void **)&result);
    	total += result;
    }

    queue_delete(q);

    printf("total = %lu\n", total);
    return 0;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
