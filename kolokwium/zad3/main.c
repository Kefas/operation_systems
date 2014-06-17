#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>


struct pamiec_wspolna{
  int cos;
  sem_t widelec[5];
};


void *filozof(void* arg){
  int shmid;
  struct pamiec_wspolna* strct;
  int numer_filozofa;

  numer_filozofa = (int) arg;

  shmid = shmget((key_t)12345, sizeof(struct pamiec_wspolna), 0666);
  strct = (struct pamiec_wspolna*) shmat(shmid, (void*)0, 0);
  
  while(1){
    sleep(rand()%3);
    printf("Filozof %d chce wziac widelec po lewej %d\n", numer_filozofa, numer_filozofa);
    sem_wait(strct->widelec+numer_filozofa);
    
    printf("Filozof %d chce wziac widelec po prawej %d\n", numer_filozofa, (numer_filozofa + 1) %5);
    sem_wait(strct->widelec+((numer_filozofa+1)%5));

    printf("filozof %d je\n", numer_filozofa);
    sleep(2);
    sem_post(strct->widelec+numer_filozofa);
    sem_post(strct->widelec+((numer_filozofa+1)%5));
    
  }





}

int main(void){

  pthread_t threads[5];
  int i;
  int shmid;
  struct pamiec_wspolna* strct;

  shmid = shmget((key_t)12345, sizeof(struct pamiec_wspolna), IPC_CREAT|0666);
  strct = (struct pamiec_wspolna*) shmat(shmid, (void*)0, 0);
 
  
 strct->cos = 0;

  srand(time(NULL));
  for(i=0;i<5;i++)
    sem_init(&(strct->widelec[i]),0,1);

  for(i=0;i<5;i++)
    pthread_create(threads+i,NULL, filozof, (void*) i);


  pthread_join(threads[0], (void**) 0);
  /* shmdt(strct); */
  
  return 0;
}
