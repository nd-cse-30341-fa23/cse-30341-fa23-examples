/* counter.c: counter shell (event) */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <poll.h>
#include <unistd.h>

/* Constants */

const char *PROMPT  = "> ";
const int   TIMEOUT = 1000;

/* Main Execution */

int main(int argc, char *argv[]) {
    bool prompt  = true;
    int  counter = 0;
    char buffer[BUFSIZ];

    while (true) {
        /* Display prompt */
        if (prompt) {
            fputs(PROMPT, stdout); fflush(stdout); prompt = false;
        }

        /* Poll STDIN */
        struct pollfd pfd = {STDIN_FILENO, POLLIN|POLLPRI, 0};
        int    result     = poll(&pfd, 1, TIMEOUT);

        /* Process event */
        if (result < 0) {           /* Error */
            fprintf(stderr, "Unable to poll: %s\n", strerror(errno));
        } else if (result == 0) {   /* Update counter */
            counter++;
        } else {                    /* Handle command */
            if (!fgets(buffer, BUFSIZ, stdin)) {
                break;
            }

            buffer[strlen(buffer) - 1] = 0;

            if (strcmp(buffer, "count") == 0) {
                fprintf(stdout, "Counter is %d\n", counter);
            } else if (strcmp(buffer, "reset") == 0) {
                counter = 0;
                fputs("Counter is reset!\n", stdout);
            } else if (strcmp(buffer, "exit") == 0) {
                fputs("Bye!\n", stdout);
                break;
            } else {
                fprintf(stderr, "Unknown command: %s\n", buffer);
            }

            prompt = true;
        }
    }

    return 0;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
