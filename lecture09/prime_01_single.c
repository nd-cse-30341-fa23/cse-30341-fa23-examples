/* prime_single.c: brute-force prime finder (single-threaded) */

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

/* Macros */

#define max(a, b)   ((a) > (b) ? (a) : (b))

/* Constants */

const size_t PRIME_MAX = 400000;

/* Functions */

bool is_prime(size_t n) {
    for (size_t i = 2; i < n; i++) {
    	if (n % i == 0)
    	    return false;
    }
    return true;
}

/* Main execution */

int main(int argc, char *argv[]) {
    size_t count = 0;
    for (size_t i = 2; i < PRIME_MAX; i++) {
	count += is_prime(i);
    }
    printf("There are %lu primes less than %lu\n", count, PRIME_MAX);
    return 0;
}
