CC = gcc
FLAGS =  -oterm -pthread

hello: hello.c 
	$(CC) $(FLAGS) $^ -o hello

func: func.c
	$(CC) $(FLAGS) $^ -o func

cond: cond.c
	$(CC) $(FLAGS) $^ -o cond
clean:
	rm -f *.o
	rm -f *~
	rm -f *.bak