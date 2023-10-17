/* malloc.c: simple memory allocator */

#include <assert.h>
#include <unistd.h>

/* Allocate space */

void *malloc(size_t size) {
    void *block = sbrk(size);

    if (block == (void *)-1) {    /* OS allocation failed */
    	return NULL;
    }

    return block;
}

/* Reclaim space */

void free(void *ptr) {
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
