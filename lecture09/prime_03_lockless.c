/* prime_lockless.c: brute-force prime finder (lockless) */

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

/* Functions */

size_t is_prime(size_t n) {
    for (size_t i = 2; i < n; i++) {
    	if (n % i == 0)
    	    return 0;
    }
    return 1;
}

/* Threads */

void * count_primes(void *arg) {
    PrimeArgs *pa = (PrimeArgs *)arg;
    size_t  count = 0;
    printf("start=%lu, stop=%lu\n", pa->start, pa->stop);
    for (size_t n = pa->start; n < pa->stop; n++) {
    	count += is_prime(n);
    }
    return (void *)count;
}

int main(int argc, char *argv[]) {
    pthread_t threads[NCPUS];		    // Array of threads
    PrimeArgs args[NCPUS];		    // Array of arguments
    size_t count = 0, result;

    for (size_t i = 0; i < NCPUS; i++) {    // Division of work
    	args[i].start = max(2, i*PRIME_MAX/NCPUS);
    	args[i].stop  = (i+1)*PRIME_MAX/NCPUS;
    	pthread_create(&threads[i], NULL, count_primes, &args[i]);
    }

    for (size_t i = 0; i < NCPUS; i++) {    // Wait for threads
    	pthread_join(threads[i], (void **)&result);
    	count += result;
    }

    printf("There are %lu primes less than %ld\n", count, PRIME_MAX);
    return 0;
}
