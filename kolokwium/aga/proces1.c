#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>



int main(void){
  int shmid, semid;
  char* napis; 

  struct sembuf buf;


  if((shmid = shmget((key_t)1234, 100, IPC_CREAT |0666)) == -1){
    perror("shmget proces1");
    return 1;
  }

  if((napis = (char*) shmat(shmid, (void*)0, 0)) == -1){
    perror("shmat");
    return 1;
  }

  if((semid = semget((key_t)1111, 1, IPC_CREAT|0666)) == -1){
    perror("semid");
    return 1;
  }

  if((semctl(semid, 0, SETVAL, 1)) == -1){
    perror("semctl");
    return 1;
  }

  buf.sem_num = 0;
  buf.sem_op = -1;
  buf.sem_flg = 0;

  if(semop(semid, &buf,2) == -1) {
    perror("semop opuszczenie");
    return 1;
  }

  printf("Przed wyslaniem do pamieci wspl. %s\n", napis);
  strcpy(napis, "Witam");
  printf("Pierwszy proces %s\n", napis);

  buf.sem_num = 0;
  buf.sem_op = 1;
  buf.sem_flg = 0;

  if(semop(semid, &buf,2) == -1) {
    perror("semop podnoszenie");
    return 1;
  }
  

  if(shmdt(napis) == -1){
    perror("semdt");
    return 1;
  }

  return 0;
}
