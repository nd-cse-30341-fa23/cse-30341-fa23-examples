/* hello-n.c */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
    	fprintf(stderr, "Usage: %s N\n", argv[0]);
    	return EXIT_FAILURE;
    }

    int n = strtol(argv[1], NULL, 10);

    for (int i = 0; i < n; i++) {
    	pid_t pid = fork();
    	if (pid < 0) {	// Error
	    fprintf(stderr, "fork failed: %s\n", strerror(errno));
    	    return EXIT_FAILURE;
	}

    	if (pid == 0) {	// Child
    	    sleep(i);
    	    printf("Hello from child %d\n", i);
    	    exit(i);
	} /*else {	// Version 2: Parent waits after each child (serial)
	    while (wait(NULL) < 0);
	}
	*/

    }

    // Version 1: No wait at all... children keep running when parent exits

    // Version 3: Wait after launching all children (parallel)
    for (int i = 0; i < n; i++) {
    	int status;
    	pid_t pid;
	while ((pid = wait(&status)) < 0);
	printf("Child %d with exit status: %d\n", pid, WEXITSTATUS(status));
    }

    return EXIT_SUCCESS;
}
