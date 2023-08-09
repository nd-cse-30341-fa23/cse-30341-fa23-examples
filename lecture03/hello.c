#include <stdlib.h>
#include <unistd.h>

int main() {
    write(STDOUT_FILENO, "hello, world\n", 13);
    exit(0);
}
