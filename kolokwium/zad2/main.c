#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <pthread.h>
#include "message.h"



int main(int argc, char* argv[]){
  
  int id, pid;
  struct message msg;
  int bread=0;
  //struct msgbuf* msg;

  if((id = msgget((key_t)12345,  IPC_CREAT|0660)) == -1){
    perror("msgget");
    return 1;
  }


  msg.type = 1;
  msg.tab[0] = 1.5;
  msg.tab[1] = 1.5;

  printf("Podaj pierwszego floata: ");
  scanf("%f", msg.tab);
  printf("\n Podaj drugiego floata: ");
  scanf("%f", msg.tab+1);

  printf("klient wysyla\n");
  if(msgsnd(id, &msg, sizeof(struct message), 0) == -1){
    perror("msgsnd");
    return 1;
  }

    
  printf("klient odbiera\n");
  while(bread == 0){
    if((bread = msgrcv(id, &msg, sizeof(struct message), 2, 0)) == -1){
      perror("msgrvc");
      return 1;
    }
    printf("Po odebraniu bread %d\n", bread);
    printf("%f * %f = %f", msg.tab[0], msg.tab[1], msg.result);
  }

  msgctl(id, IPC_RMID, 0);
  return 0;
}
