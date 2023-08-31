TARGET=		pc0 pc1 pc2 pc3
CFLAGS=		-g -Wall -std=gnu99 -pthread 

all:		$(TARGET)

pc0:		queue0.c pc.c
	$(CC) $(CFLAGS) -o $@ $^

pc1:		queue1.c pc.c
	$(CC) $(CFLAGS) -o $@ $^

pc2:		queue2.c pc.c
	$(CC) $(CFLAGS) -o $@ $^

pc3:		queue3.c pc.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
