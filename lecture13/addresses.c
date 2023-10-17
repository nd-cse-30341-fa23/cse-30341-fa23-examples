// addresses.c

#include <stdio.h>
#include <stdlib.h>

int Global = 0;

int main(int argc, char *argv[]) {
    int local = 1;

    printf("Location of STACK: %14p\n", &local);
    printf("Location of HEAP:  %14p\n", malloc(1));
    printf("Location of DATA:  %14p\n", &Global);
    printf("Location of CODE:  %14p\n", main);

    return EXIT_SUCCESS;
}
