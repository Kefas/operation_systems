#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

#define N_MAN 15
#define N_WOMAN 15

sem_t bathroom, sex;

pthread_mutex_t m_man, m_woman;

int n_man=0,n_woman=0;

void* man(void* arg){
  while(1){
    sleep(rand()%10 );
    printf("----\nMan %d want to go in\n----\n", (int)arg);
    pthread_mutex_lock(&m_man);
    if(n_man == 0)
      sem_wait(&sex);
    pthread_mutex_unlock(&m_man);
    sem_wait(&bathroom);
    pthread_mutex_lock(&m_man);
    n_man++;
    printf("Man %d is in, men: %d\n", (int)arg, n_man);
    pthread_mutex_unlock(&m_man);
    
    sleep(rand()%2);

    pthread_mutex_lock(&m_man);
    sem_post(&bathroom);
    
    n_man--;
   
    printf("Man %d go out\n", (int) arg);
    if(n_man == 0)
      sem_post(&sex);
    pthread_mutex_unlock(&m_man);
    
  }

}

void* woman(void* arg){
  int value;
  while(1){
    sleep(rand()%10 );
    printf("----\nWoman %d want to go in\n----\n", (int)arg);
    pthread_mutex_lock(&m_woman);
    if(n_woman == 0)
      sem_wait(&sex);
    /* sem_getvalue(&bathroom, &value); */
    /* printf("semafor is %d\n", value); */
    pthread_mutex_unlock(&m_woman);
    sem_wait(&bathroom);
    pthread_mutex_lock(&m_woman);
    n_woman++;
    pthread_mutex_unlock(&m_woman);
    printf("Woman %d is in,women: %d\n", (int) arg, n_woman);
    

    sleep(rand()%2);
    
    

    pthread_mutex_lock(&m_woman);
    sem_post(&bathroom);
    n_woman--;
   
    if(n_woman == 0)
      sem_post(&sex);
    printf("Woman %d go out\n", (int) arg);
    pthread_mutex_unlock(&m_woman);
  }

}



int main(int argc, char* argv[]){
  int i;
  pthread_t threads[N_MAN + N_WOMAN];
  pthread_mutex_init(&m_man, NULL);
  pthread_mutex_init(&m_woman, NULL);
  sem_init(&bathroom, 0, 3);
  sem_init(&sex,0,1);
  srand(time(NULL));
  

  for(i=0;i<N_MAN;i++)
    if((pthread_create(threads+i,NULL, man,(void *) i))==-1){
      perror("Thread creation MAN");
      exit(EXIT_FAILURE);
    }

  for(i=0;i<N_WOMAN;i++)
    if((pthread_create(threads+i,NULL, woman,(void *) i))==-1){
      perror("Thread creation WOMAN");
      exit(EXIT_FAILURE);
    }

  for(i=0;i<N_WOMAN+N_MAN;i++)
    if((pthread_join(threads[i],NULL))==-1){
       perror("Thread join");
       exit(EXIT_FAILURE);
    }
  return 0;
}
