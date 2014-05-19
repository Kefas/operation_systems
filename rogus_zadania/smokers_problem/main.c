#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>
#include <semaphore.h>

/*
  0 - paper
  1 - tobacco
  2 - matches
*/

pthread_mutex_t turn[3], table;
int ingredients[3] = {0,0,0};
sem_t sem;


void* smoker(void* arg){
  int i;
  while(1){
    pthread_mutex_lock(turn + (int) arg);
    pthread_mutex_lock(&table);

    if(ingredients[(int)arg] == 0 && (ingredients[0] || ingredients[1])){
      printf("Smoker %d smoke\n", (int) arg);
      sleep(1);
      for(i=0;i<3;i++)
	ingredients[i]=0;
    }

    sem_post(&sem);
    pthread_mutex_unlock(&table);
    pthread_mutex_unlock(turn + (int) arg);
    
   
  }

}

void* lackey(void * arg){
  int i;
  int last=10;
  int random=-1;
  while(1){
    printf("Hello and die\n");

    sem_wait(&sem);
    sem_wait(&sem);
    sem_wait(&sem);

    random = rand()%3;
    last = random;
    ingredients[random] = 1;
    random = rand()%3;
    if(last == random)
      if(random == 0)
	random = 1;
      else
	random = 0;
    ingredients[random] = 1;

    printf("----------------\nLackey put %d i %d\n", last, random);
    sleep(2);
    pthread_mutex_unlock(turn);
    pthread_mutex_unlock(turn+1);
    pthread_mutex_unlock(turn+2);
  }
}    



int main(int argc, char* argv[]){

  pthread_t threads[4];
  int i;
  srand(time(NULL));
  for(i=0;i<3;i++)
    pthread_mutex_init(turn + i, NULL);
  pthread_mutex_init(&table, NULL);
  sem_init(&sem,0,3);
  
  for(i=0;i<3;i++)
    pthread_create(threads+i, NULL, smoker, (void*) i);
  pthread_create(threads+3, NULL, lackey, NULL);

  for(i=0;i<4;i++)
    pthread_join(threads[i], NULL);


  return 0;
}
