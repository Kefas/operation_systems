#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h> 
#include <pthread.h>

#include "message.h"

#define X1 2
#define X2 2
#define X3 2
#define X4 2


int sem[4];
int def_val[] = {X1,X2,X3,X4};

void* client(void* arg){
  int fdsrv, fdcnt;
  int group_size;
  int bwrite, bread;
  struct sembuf sem_buf;
  struct bunch bun;
  struct response resp;
  int i;
  char* buf = (char*) malloc(sizeof(char) * 3);
  sprintf(buf,"%d",(int)arg);
  
  if(mkfifo(buf, S_IFIFO|0755) == -1){
    perror("Tworzenie kolejki klienta");
    pthread_exit((void*)EXIT_FAILURE);
  }
 
  
  if((fdsrv = open("fifo_server", O_WRONLY)) == -1){
    perror("Otwieranie kolejki fifo_server w kliencie");
    pthread_exit((void*)EXIT_FAILURE);
  } 

  

 
  while(1){
    group_size = rand()%3 +2;
    
    

    sleep(rand()%3+3);
    bun.id = (int) arg;
    bun.size = group_size;
    if((bwrite = write(fdsrv,&bun, sizeof(bun))) == -1){
      perror("Pisanie do kolejki fifo z klienta");
      pthread_exit((void*)EXIT_FAILURE);
    }

    if((fdcnt = open(buf, O_RDONLY)) == -1){
      perror("Otwieranie kolejki klienta w kliencie");
      pthread_exit((void*)EXIT_FAILURE);
    }
    printf("Otwieranie kolejki klienta\n");
    
    
    if((bread = read(fdcnt, &resp, sizeof(resp))) == -1){
      perror("czytanie w klient");
      pthread_exit((void *) EXIT_FAILURE);
    }

    printf("Odebrałem status %d\n", resp.status);
    
    switch(resp.status){
    case 0:
      printf("%d people are eating\n", group_size);
      sleep(4);
      sem_buf.sem_num = resp.which_sem;
      sem_buf.sem_op = 1;
      sem_buf.sem_flg = 0;

      for(i=0;i<group_size;i++)
	if(semop(sem[resp.size_of_table], &sem_buf, 1) == -1){
	  perror("podnoszenie semafora");
	  pthread_exit((void*) EXIT_FAILURE);
	}

      printf("%d people left\n", group_size);
      break;
    case 1:
      printf("There is no place to sit in pizzeria!\n");
      break;
    }


    close(fdcnt);
    
  }
}


int main(int argc, char* argv[]){
  int n_threads;
  pthread_t *threads;
  int i;

  if(argc != 2){
    perror("Usage : ./client number_of_clients");
    exit(EXIT_FAILURE);
  }

  n_threads = atoi(argv[1]);
  threads = (pthread_t*) malloc(n_threads*sizeof(pthread_t));
  srand(time(NULL));

  printf("Odpalam %d watki\n", n_threads);
  for(i=0;i<n_threads;i++)
    if((pthread_create(threads+i,NULL, client, (void*) i)) == -1){
      perror("odpalanie watkow");
      exit(EXIT_FAILURE);
    }
  
  for(i=0;i<4;i++)
    if((sem[i] = semget(1111*(i+1), def_val[i], 0755)) == -1 ){
    perror("Semafor get");
    exit(EXIT_FAILURE);
  }
 
  

  while(1){


  }


  return 0;
}
