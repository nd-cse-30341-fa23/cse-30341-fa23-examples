TARGET=		pc
CFLAGS=		-g -Wall -std=gnu99 -pthread 

all:		$(TARGET)

pc:		queue.c pc.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
