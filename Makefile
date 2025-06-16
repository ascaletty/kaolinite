CC= gcc
CFLAGS= -lraylib -lm -o kaolinite -g
main: main.c
	$(CC) $(CFLAGS) main.c 
	sudo cp kaolinite /usr/bin/
	sudo cp kaolinite_run.sh /usr/bin/kaolinite_run
	sudo cp kaolinite_path.sh /usr/bin/kaolinite_path

