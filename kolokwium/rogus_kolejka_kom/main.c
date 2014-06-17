#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <pthread.h>
#include "message.h"



int main(int argc, char* argv[]){
  
  int id, pid;
  struct message msg;
  //struct msgbuf* msg;
  pthread_mutex_t mut;

  pthread_mutex_init(&mut, NULL);


  if((id = msgget((key_t)12345,  IPC_CREAT|0660)) == -1){
    perror("msgget");
    return 1;
  }

  if((pid = fork()) == -1){
    perror("fork");
    return 1;
  }
  if(pid == 0){
    while(1){
      sleep(2);
      pthread_mutex_lock(&mut);
      msg.type = 1;
      msg.tab[0] = 1.5;
      msg.tab[1] = 1.5;
      printf("klient wysyla\n");
      if(msgsnd(id, &msg, sizeof(struct message), 0) == -1){
	perror("msgsnd");
	return 1;
      }

      pthread_mutex_unlock(&mut);
    }

  }
  else{
    while(1){
      sleep(1);
      pthread_mutex_lock(&mut);
      printf("klient odbiera\n");
      if(msgrcv(id, &msg, sizeof(struct message), 2, 0) == -1){
	perror("msgrvc");
	return 1;
      }
      printf("Po odebraniu\n");
      printf("%f * %f = %f", msg.tab[0], msg.tab[1], msg.result);
      pthread_mutex_unlock(&mut);
    }

  }

  return 0;
}
