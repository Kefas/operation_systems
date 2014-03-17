CC = gcc
FLAGS = -Wall -pedantic -ansi

copy1: copy1.c
	$(CC) $(FLAGS) $^ -o copy1

.PHONY: clean

clean:
	rm -f *~
	rm -f *.bak
	rm -f *.o