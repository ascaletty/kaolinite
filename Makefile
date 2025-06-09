CC= gcc
CFLAGS= -lraylib -lm -o kaolinite -g
main: main.c
	$(CC) $(CFLAGS) main.c 
