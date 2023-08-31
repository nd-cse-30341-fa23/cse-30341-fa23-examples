/* counter.c: counter shell (threads) */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <poll.h>
#include <unistd.h>

/* Constants */

const char *PROMPT   = "> ";
const int   TIMEOUT  = 1000;

/* Globals */

int         Counter  = 0;
pthread_mutex_t Lock = PTHREAD_MUTEX_INITIALIZER;

/* Functions */

void *update_counter(void *arg) {
    while (true) {
        usleep(TIMEOUT * 1000);
        pthread_mutex_lock(&Lock);
        Counter += 1;
        pthread_mutex_unlock(&Lock);
    }

    return NULL;
}

/* Main execution */

int main(int argc, char *argv[]) {
    // Background Thread - Update Loop
    pthread_t thread;
    pthread_create(&thread, NULL, update_counter, 0);

    // Foreground Thread - I/O Loop
    while (true) {
        char buffer[BUFSIZ];

        // Display prompt
        fputs(PROMPT, stdout); fflush(stdout);

        // Read input
        if (!fgets(buffer, BUFSIZ, stdin)) {
            break;
        }
        buffer[strlen(buffer) - 1] = 0;

        // Process input
        if (strcmp(buffer, "count") == 0) {
            pthread_mutex_lock(&Lock);
            fprintf(stdout, "Counter is %d\n", Counter);
            pthread_mutex_unlock(&Lock);
        } else if (strcmp(buffer, "reset") == 0) {
            pthread_mutex_lock(&Lock);
            Counter = 0;
            pthread_mutex_unlock(&Lock);
            fputs("Counter is reset!\n", stdout);
        } else if (strcmp(buffer, "exit") == 0) {
            fputs("Bye!\n", stdout);
            break;
        } else {
            fprintf(stderr, "Unknown command: %s\n", buffer);
        }
    }

    return 0;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */ 
