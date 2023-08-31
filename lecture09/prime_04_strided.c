/* prime_strided.c: brute-force prime finder (strided) */

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

/* Macros */

#define max(a, b)   ((a) > (b) ? (a) : (b))

/* Constants */

const size_t NCPUS     = 8;
const size_t PRIME_MAX = 400000;

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
    size_t start = (size_t)arg;
    size_t count = 0;
    printf("start=%lu\n", start);
    for (size_t n = start; n < PRIME_MAX; n += NCPUS) {
    	count += is_prime(n);
    }
    return (void *)count;
}

int main(int argc, char *argv[]) {
    pthread_t threads[NCPUS];		    // Array of threads
    size_t count = 0, result;

    for (size_t i = 0; i < NCPUS; i++) {    // Division of work
    	pthread_create(&threads[i], NULL, count_primes, (void *)i);
    }

    for (size_t i = 0; i < NCPUS; i++) {    // Wait for threads
    	pthread_join(threads[i], (void **)&result);
    	count += result;
    }

    printf("There are %lu primes less than %ld\n", count, PRIME_MAX);
    return 0;
}
