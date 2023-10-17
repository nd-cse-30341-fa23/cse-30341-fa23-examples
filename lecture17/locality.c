/* locality.c: */

#include <stdio.h>
#include <stdlib.h>

#define LOOPS	(1<<10)
#define N	(1<<11)

int main(int argc, char *argv[]) {
    int **m = malloc(sizeof(int *) * N);    /* Review: Allocate 2D array */
    for (int i = 0; i < N; i++) {
    	m[i] = malloc(sizeof(int) * N);
	for (int j = 0; j < N; j++) {
	    m[i][j] = j;
	}
    }

    for (int l = 0; l < LOOPS; l++) {
	int total = 0;
	for (int i = 0; i < N; i++)
	    for (int j = 0; j < N; j++)
		total += m[i][j];	    /* Demonstrate: Swap i, j */
    }

    return EXIT_SUCCESS;
}
