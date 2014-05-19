#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h> 
#include <stdio.h>

static struct sembuf buf;
void v(int semid, int semnum){
  buf.sem_num = semnum;
  buf.sem_op = 1;
  buf.sem_flg = 0;
  if (semop(semid, &buf, 1) == -1){
    perror("Podnoszenie semafora");
    exit(1);
  }
} 

void p(int semid, int semnum){
  buf.sem_num = semnum;
  buf.sem_op = -1;
  buf.sem_flg = 0;
  if (semop(semid, &buf, 1) == -1){
    perror("Opuszczenie semafora");
    exit(1);
  }
}
