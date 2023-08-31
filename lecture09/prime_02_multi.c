/* prime_multi.c: brute-force prime finder (multi-threaded) */

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

/* Macros */

#define max(a, b)   ((a) > (b) ? (a) : (b))

/* Constants */

const size_t NCPUS     = 8;
const size_t PRIME_MAX = 400000;

/* Structure */

typedef struct {
    size_t start;
    size_t stop;
} PrimeArgs;

/* Global Variables */

size_t          Count = 0;
pthread_mutex_t Lock  = PTHREAD_MUTEX_INITIALIZER;

/* Functions */

size_t is_prime(size_t n) {
    for (size_t i = 2; i < n; i++) {
    	if (n % i == 0)
    	    return 0;
    }
    return 1;
}

/* Threads */

void * count_primes_1(void *arg) {	    // Version 1: lock each check
    PrimeArgs *pa = (PrimeArgs *)arg;
    printf("start=%lu, stop=%lu\n", pa->start, pa->stop);
    for (size_t n = pa->start; n < pa->stop; n++) {
    	pthread_mutex_lock(&Lock);
    	Count += is_prime(n);
    	pthread_mutex_unlock(&Lock);
    }
    return NULL;
}

void * count_primes_2(void *arg) {	    // Version 2: only lock once
    PrimeArgs *pa = (PrimeArgs *)arg;
    size_t  count = 0;
    printf("start=%lu, stop=%lu\n", pa->start, pa->stop);
    for (size_t n = pa->start; n < pa->stop; n++) {
    	count += is_prime(n);
    }
    pthread_mutex_lock(&Lock);
    Count += count;
    pthread_mutex_unlock(&Lock);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t threads[NCPUS];		    // Array of threads
    PrimeArgs args[NCPUS];		    // Array of arguments

    for (size_t i = 0; i < NCPUS; i++) {    // Division of work
    	args[i].start = max(2, i*PRIME_MAX/NCPUS);
    	args[i].stop  = (i+1)*PRIME_MAX/NCPUS;
    	pthread_create(&threads[i], NULL, count_primes_2, &args[i]);
    }

    for (size_t i = 0; i < NCPUS; i++) {    // Wait for threads
    	pthread_join(threads[i], NULL);
    }

    printf("There are %lu primes less than %ld\n", Count, PRIME_MAX);
    return 0;
}
