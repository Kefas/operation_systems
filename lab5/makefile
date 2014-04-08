CC = gcc
FLAGS =  -oterm -pthread

hello: hello.c 
	$(CC) $(FLAGS) $^ -o hello

func: func.c
	$(CC) $(FLAGS) $^ -o func

cond: cond.c
	$(CC) $(FLAGS) $^ -o cond

randomsearch: randomsearch.c
	$(CC) $(FLAGS) $^ -o randomsearch


clean:
	rm -f *.o
	rm -f *~
	rm -f *.bak