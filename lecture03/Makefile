SOURCE=		hello.c hello-asm.s
TARGET=		hello hello-asm

all:		$(TARGET)

%:		%.c
	gcc -Wall -g -std=gnu99 -o $@ $<

%:		%.s
	as -o $@.o $<
	ld -o $@ -e main $@.o 

clean:
	rm -f $(TARGET) *.o
