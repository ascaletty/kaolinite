CC= gcc
CFLAGS= -lraylib -lm -o output.o -g
main: main.c
	$(CC) $(CFLAGS) main.c 
