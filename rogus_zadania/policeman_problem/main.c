#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>
#include <semaphore.h>

#define MAX_CON 20

int num_con=0;
pthread_mutex_t room;
pthread_mutex_t m_policeman;
pthread_mutex_t secure;

void* policeman(void* arg){
  while(1){
   /* printf("POL Zamykam secure\n"); */
    pthread_mutex_lock(&secure);
    if(num_con == 0 || num_con == 7){
      /* printf("POL Otwieram secure\n"); */
      pthread_mutex_unlock(&secure);
      printf("Policeman in room!\n");
      /* printf(" POL zamykam m_policeman\n"); */
      pthread_mutex_lock(&m_policeman);
      /* printf("POL Zamykam room\n"); */
      pthread_mutex_lock(&room);
      printf("Policemen gone!\n");
      
      if(num_con == 0){
	/* printf("POL otwieram room\n"); */
      pthread_mutex_unlock(&room);
      /* printf("POL otwieram policjanata\n"); */
      pthread_mutex_unlock(&m_policeman);
      }
    }
    pthread_mutex_unlock(&secure);
    sleep(rand()%2);
  }

}

void* conspirator(void* arg){
  
  while(1){
    sleep(rand()%3+1);
    /* printf("zamykam m_policeman\n"); */
    pthread_mutex_lock(&m_policeman);
    /* printf("Zamykam secure\n");   */
    pthread_mutex_lock(&secure);
    if(num_con<MAX_CON){
      num_con++;
      printf("There is %d conspirators in room!\n", num_con);
      if(num_con == 1){
	/* printf("Zamykam room\n"); */
	pthread_mutex_lock(&room);
      }
    }
    /* printf("otwieram m_policeman\n"); */
    pthread_mutex_unlock(&m_policeman);
    /* printf("Otwieram secure\n"); */
    pthread_mutex_unlock(&secure);

    sleep(rand()%2+1);
    /* printf("Zamykam secure\n"); */
    pthread_mutex_lock(&secure);
    if(num_con > 0){
      num_con--;
      printf("Conspirator left now is %d \n", num_con);
      if(num_con == 0){
	/* printf("Otwieram room\n"); */
	pthread_mutex_unlock(&room);
      }
    }
    /* printf("Otwieram secure\n"); */
    pthread_mutex_unlock(&secure);
    
  }
}


int main(int argc, char* argv[]){

  pthread_t threads[MAX_CON + 1];
  int i;
  pthread_mutex_init(&m_policeman, NULL);
  pthread_mutex_init(&room, NULL);
  pthread_mutex_init(&secure, NULL);
  srand(time(NULL));
  for(i=0;i<MAX_CON; i++)
    pthread_create(threads + i, NULL, (void*)conspirator, (void*) i);
  pthread_create(threads + MAX_CON, NULL, (void*) policeman, (void*)(MAX_CON));

  for(i=0;i<MAX_CON+1;i++)
    pthread_join(threads[i], NULL);


  return 0;
}
