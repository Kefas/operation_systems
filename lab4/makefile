CC = gcc
FLAGS = -pedantic -Wall -ansi

shell: shell.c funcs.c
	$(CC) $(FLAGS) $^ -o shell

clean:
	rm -f *.o
	rm -f *~
	rm -f *.bak