#include <stdbool.h>
#include <stdio.h>

#define ALIGN(s)   (((((s) - 1) >> 2) << 2) + 4)

struct block {
    size_t        size;
    struct block *next;
    bool          free;
};

int main(int argc, char *argv[]) {
    struct block b;

    printf("block size: %lu\n", sizeof(b));
    for (size_t i = 0; i < 64; i++) {
	printf("ALIGN(%lu): %lu\n", i, ALIGN(i));
    }
    return 0;
}
