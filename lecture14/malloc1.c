/* malloc.c: POSIX API Implementation */

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Block Constants */

#define ALIGNMENT       (sizeof(double))
#define ALIGN(size)     (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define SBRK_FAILURE    ((void *)(-1))

/* Block Macros */

#define BLOCK_FROM_POINTER(ptr) \
    (Block *)((intptr_t)(ptr) - sizeof(Block))

/* Block Structure */

typedef struct block Block;
struct block {
    size_t   capacity;  /* Number of bytes allocated to block */
    size_t   size;      /* Number of bytes used by block */
    Block *  prev;      /* Pointer to previous block structure */
    Block *  next;      /* Pointer to next block structure */
    char     data[];    /* Label for user accessible block data */
};

Block * block_allocate(size_t size) {
    intptr_t allocated = sizeof(Block) + ALIGN(size);
    Block *  block     = sbrk(allocated);
    if (block == SBRK_FAILURE) {
        return NULL;
    }

    block->capacity = ALIGN(size);
    block->size     = size;
    block->prev     = block;
    block->next     = block;
    return block;
}

Block * block_detach(Block *block) {
    /* Update prev and next blocks to reference each other */
    if (block) {
        Block *prev = block->prev;
        Block *next = block->next;
        prev->next  = block->next;
        next->prev  = block->prev;
        block->next = block;
        block->prev = block;
    }

    return block;
}

/* Free List Global Variable */

Block FreeList = {-1, -1, &FreeList, &FreeList};

/* Free List Functions */

Block * free_list_search(size_t size) {
    /* Perform first fit algorithm */
    for (Block *curr = FreeList.next; curr != &FreeList; curr = curr->next) {
        if (curr->capacity >= size) {
            return curr;
        }
    }

    return NULL;
}

void    free_list_insert(Block *block) {
    /* Append to tail */
    Block *tail   = FreeList.prev;
    tail->next    = block;
    FreeList.prev = block;
    block->next   = &FreeList;
    block->prev   = tail;
}

/* POSIX API Functions */

/**
 * Allocate specified amount memory.
 * @param   size    Amount of bytes to allocate.
 * @return  Pointer to the requested amount of memory.
 **/
void *malloc(size_t size) {
    /* Handle empty size */
    if (!size) {
        return NULL;
    }

    // Search free list for any available block with matching size
    Block *block = free_list_search(size);

    if (!block) {
        block = block_allocate(size);
    } else {
        block = block_detach(block);
    }

    /* Could not find free block or allocate a block, so just return NULL */
    if (!block) {
        return NULL;
    }

    /* Return data address associated with block */
    return block->data;
}

/**
 * Release previously allocated memory.
 * @param   ptr     Pointer to previously allocated memory.
 **/
void free(void *ptr) {
    if (!ptr) {
        return;
    }

    Block *block = BLOCK_FROM_POINTER(ptr);
    free_list_insert(block);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
