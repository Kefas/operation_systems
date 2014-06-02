#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <sys/sem.h>

#include "message.h"

#define X1 2
#define X2 2
#define X3 2
#define X4 2


sendResponse(struct bunch msg, int sem[4]){
  struct response resp;
  int fdcnt;
  int bwrite;
  int i,j;
  int ret;
  int flag=0;
  char* buf = (char*) malloc(3 * sizeof(char));
  struct sembuf sem_buf;

  sprintf(buf,"%d",msg.id);
  /* wlasciciel okresla czy grupa moze usiasc i opuszcza semafory jezeli moze*/
   
  if((fdcnt = open(buf, O_WRONLY)) == -1){
    perror("Otwieranie kolejki klienta w ownerze");
    pthread_exit((void*)EXIT_FAILURE);
  } 
  
  
  switch(msg.size){
  case 4:
    printf("Stolik dla czworga!\n");
    for(i=0;i<X4;i++){
      if( (ret = semctl(sem[3], i, GETVAL,0)) == -1){
	perror("semctl\n");
       	exit(EXIT_FAILURE);
      }
      if(ret == 4){
	printf("jest ok stolik czworka\n");
	sem_buf.sem_num = i;
	sem_buf.sem_op = -1;
	sem_buf.sem_flg = 0;

	for(j=0;j<ret;j++){
	  if(semop(sem[3], &sem_buf, 1) == -1){
	    perror("opuszczanie semafora");
	    exit(EXIT_FAILURE);
	  }
	}
	flag = 1;
	resp.status = 0;
	resp.size_of_table = 3;
	resp.which_sem = i;
	if((bwrite = write(fdcnt, &resp, sizeof(resp))) == -1){
	  perror("Sending response from owner");
	  exit(EXIT_FAILURE);
	}
	break;
      }
    }
    if(flag == 0){
      resp.status = 1;
      resp.size_of_table = 3;
      if((bwrite = write(fdcnt, &resp, sizeof(resp))) == -1){
	perror("Sending response from owner");
	exit(EXIT_FAILURE);
      }
    }
    flag = 0;
    break;
  case 3:
    printf("Stolik dla trojki!");
    for(i=0;i<X3;i++){
      if( (ret = semctl(sem[2], i, GETVAL,0)) == -1){
	perror("semctl\n");
       	exit(EXIT_FAILURE);
      }
      if(ret == 3){
	printf("jest ok stolik trojka\n");
	sem_buf.sem_num = i;
	sem_buf.sem_op = -1;
	sem_buf.sem_flg = 0;

	for(j=0;j<ret;j++){
	  if(semop(sem[2], &sem_buf, 1) == -1){
	    perror("opuszczanie semafora");
	    exit(EXIT_FAILURE);
	  }
	}
	flag = 1;
	resp.status = 0;
	resp.size_of_table = 2;
	resp.which_sem = i;
	if((bwrite = write(fdcnt, &resp, sizeof(resp))) == -1){
	  perror("Sending response from owner");
	  exit(EXIT_FAILURE);
	}
	break;
      }
    }
    if(flag == 0){
      for(i=0;i<X4;i++){
	if( (ret = semctl(sem[3], i, GETVAL,0)) == -1){
	  perror("semctl\n");
	  exit(EXIT_FAILURE);
	}
	if(ret == 4){
	  printf("jest ok stolik czworka\n");
	  sem_buf.sem_num = i;
	  sem_buf.sem_op = -1;
	  sem_buf.sem_flg = 0;
	
	  for(j=0;j<ret;j++){
	    if(semop(sem[3], &sem_buf, 1) == -1){
	      perror("opuszczanie semafora");
	      exit(EXIT_FAILURE);
	    }
	  }
	  flag = 1;
	  resp.status = 0;
	  resp.size_of_table = 3;
	  resp.which_sem = i;
	  if((bwrite = write(fdcnt, &resp, sizeof(resp))) == -1){
	    perror("Sending response from owner");
	    exit(EXIT_FAILURE);
	  }
	  break;
	}
      }
      if( flag == 0){
	resp.status = 1;
	resp.size_of_table = 3;
	if((bwrite = write(fdcnt, &resp, sizeof(resp))) == -1){
	  perror("Sending response from owner");
	  exit(EXIT_FAILURE);
	}
      }
    }
    flag = 0;
    break;
  case 2:
    for(i=0;i<X2;i++){
      if( (ret = semctl(sem[1], i, GETVAL,0)) == -1){
	perror("semctl\n");
       	exit(EXIT_FAILURE);
      }
      if(ret == 2){
	printf("jest ok stolik dwojka\n");
	sem_buf.sem_num = i;
	sem_buf.sem_op = -1;
	sem_buf.sem_flg = 0;

	for(j=0;j<ret;j++){
	  if(semop(sem[1], &sem_buf, 1) == -1){
	    perror("opuszczanie semafora");
	    exit(EXIT_FAILURE);
	  }
	}
	flag = 1;
	resp.status = 0;
	resp.size_of_table = 1;
	resp.which_sem = i;
	if((bwrite = write(fdcnt, &resp, sizeof(resp))) == -1){
	  perror("Sending response from owner");
	  exit(EXIT_FAILURE);
	}
	break;
      }
    }
    if(flag == 0){
      for(i=0;i<X3;i++){
	if( (ret = semctl(sem[2], i, GETVAL,0)) == -1){
	  perror("semctl\n");
	  exit(EXIT_FAILURE);
	}
	if(ret == 3){
	  printf("jest ok stolik trojka\n");
	  sem_buf.sem_num = i;
	  sem_buf.sem_op = -1;
	  sem_buf.sem_flg = 0;
	  
	  for(j=0;j<ret;j++){
	    if(semop(sem[2], &sem_buf, 1) == -1){
	      perror("opuszczanie semafora");
	      exit(EXIT_FAILURE);
	    }
	  }
	  flag = 1;
	  resp.status = 0;
	  resp.size_of_table = 2;
	  resp.which_sem = i;
	  if((bwrite = write(fdcnt, &resp, sizeof(resp))) == -1){
	    perror("Sending response from owner");
	    exit(EXIT_FAILURE);
	  }
	  break;	  
	}
      }
    }
    if(flag == 0){
      for(i=0;i<X4;i++){
	if( (ret = semctl(sem[2], i, GETVAL,0)) == -1){
	  perror("semctl\n");
	  exit(EXIT_FAILURE);
	}
	if(ret == 2){
	  printf("jest ok stolik czworka\n");
	  sem_buf.sem_num = i;
	  sem_buf.sem_op = -1;
	  sem_buf.sem_flg = 0;
	  
	  for(j=0;j<ret;j++){
	    if(semop(sem[2], &sem_buf, 1) == -1){
	      perror("opuszczanie semafora");
	      exit(EXIT_FAILURE);
	    }
	  } 
	  flag = 1;
	  resp.status = 0;
	  resp.size_of_table = 3;
	  resp.which_sem = i;
	  if((bwrite = write(fdcnt, &resp, sizeof(resp))) == -1){
	    perror("Sending response from owner");
	    exit(EXIT_FAILURE);
	  }
	  break;	  
	  
	}
      }
      if(flag == 0){
	resp.status = 1;
	resp.size_of_table = 3;
	if((bwrite = write(fdcnt, &resp, sizeof(resp))) == -1){
	  perror("Sending response from owner");
	  exit(EXIT_FAILURE);
	}
      }
      break;
    }
    
  
  }

}
  int main(void){

    int fdsrv, bread;
    struct bunch msg;
    int i=0;
    int j=0;
    int ret;
    int wynik;
    int sem[4];
    int def_val[] = {X1,X2,X3,X4};

    if(mkfifo("fifo_server", S_IFIFO|0755) == -1){
      perror("Tworzenie fifo_server");
      exit(EXIT_FAILURE);
    }
  

    for(i=0;i<4;i++)
      if((sem[i] = semget(1111*(i+1), def_val[i], IPC_CREAT|0755)) == -1 ){
	perror("Semafor create");
	exit(EXIT_FAILURE);
      }

    for(i=0;i<4;i++)
      for(j=0;j<def_val[i];j++)
	if(semctl(sem[i], j, SETVAL, i+1) == -1){
	  perror("Nadawanie wartosci sem");
	  exit(EXIT_FAILURE);
	}

    if((fdsrv = open("fifo_server", O_RDONLY)) == -1){
      perror("Otwieranie kolejki fifo_server");
      exit(EXIT_FAILURE);
    }

    while(1){

      bread = read(fdsrv, &msg, sizeof(msg));
      if(bread < 0){
	perror("blad read in while(1)");
	exit(EXIT_FAILURE);
      }
      if(bread > 0){
	printf("Bunch %d\n",i++);
	printf("Message received from client %d\n", msg.id);
	printf("There is %d people who wants to eat\n", msg.size);
	sendResponse(msg, sem);
	for(i=0;i<4;i++){
	  for(j=0;j<def_val[i];j++){
	    if( (ret = semctl(sem[i], j, GETVAL,0)) == -1){
	      perror("semctl\n");
	      exit(EXIT_FAILURE);
	    }
	    wynik = def_val[i] - ret;
	    wynik = (wynik <0) ? 0 : wynik; 
	    printf("Stolik dla %d, zajete %d\n", i+1, wynik);
	  }

	}
      }
    

    }


    return 0;
  }
