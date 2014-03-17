CC = gcc
FLAGS = -Wall -pedantic -ansi

copy1: copy1.c
	$(CC) $(FLAGS) $^ -o copy1


cat: cat.c
	$(CC) $(FLAGS) $^ -o cat

backward: backward.c
	$(CC) $(FLAGS) $^ -o backward


.PHONY: clean

clean:
	rm -f *~
	rm -f *.bak
	rm -f *.o