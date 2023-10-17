#include <stdio.h>
#include <stdlib.h>

#define N 30

int main(int argc, char *argv[]) {
    while (1) {
	int *a = malloc(sizeof(int)*(1<<N));

	//for (int i = 0; i < 1<<N; i++) a[i] = i;
    }

    return 0;
}
