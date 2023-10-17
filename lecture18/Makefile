CC=       	gcc
CFLAGS= 	-g -std=gnu99 -Wall
LDFLAGS=	
C_SOURCE=       $(wildcard *.c)
PROGRAMS=       $(C_SOURCE:.c=)

all:    $(PROGRAMS)

%:      %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(PROGRAMS)

.PHONY: all clean
