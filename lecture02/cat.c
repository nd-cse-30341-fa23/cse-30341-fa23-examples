/* cat.c */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int cat_fd(int fd) {
    char buffer[BUFSIZ];
    int nread;

    while ((nread = read(fd, buffer, BUFSIZ)) > 0) {
    	if (nread < 0) {
    	    return EXIT_FAILURE;
	}

	int nwritten = 0;
	do {
	   int nwrote = write(STDOUT_FILENO, buffer, nread);
	   if (nwrote < 0) {
	       return EXIT_FAILURE;
	   }
	   nwritten += nwrote;
	} while (nwritten < nread);
    }

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
    	return cat_fd(STDIN_FILENO);
    }

    int result = 0;
    for (int i = 1; i < argc; i++) {
    	int fd = open(argv[i], O_RDONLY);
    	if (fd < 0) {
    	    fprintf(stderr, "Unable to open %s: %s\n", argv[i], strerror(errno));
    	    return EXIT_FAILURE;
	}

	result += cat_fd(fd);
	close(fd);
    }

    return result;
}
