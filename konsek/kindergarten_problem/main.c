#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

#define MAX_CHILD 30
#define MAX_ADULT 10

int n_adults=0, n_childs=0;

pthread_mutex_t m_adult, m_child, m_enter;
sem_t sem;

void* child(void* arg){
  int in=0;
  int x;
  while(1){
    sleep(rand()%3);
    sem_wait(&sem);
    pthread_mutex_lock(&m_enter);
    printf("Child come in\n");
    n_childs++;
    pthread_mutex_unlock(&m_enter);
   
    sleep(rand()%4);

    pthread_mutex_lock(&m_enter);
    printf("Child go out\n");
    n_childs--;
    pthread_mutex_unlock(&m_enter);
    sem_post(&sem);
  }

}

void* adult(void* arg){
  int in=0;
  int x;
  while(1){
    sleep(rand()%3);
    
    
    pthread_mutex_lock(&m_enter);
    printf("Adult come in\n");
    sem_post(&sem);
    sem_post(&sem);
    sem_post(&sem);
    n_adults++;
    pthread_mutex_unlock(&m_enter);

    printf("There is %d child and %d adults, it's %.2f child/adults\n", n_childs, n_adults, (float)n_childs/n_adults);
    sleep(rand()%4);
    
    sem_wait(&sem);
    sem_wait(&sem);
    sem_wait(&sem);

    
    pthread_mutex_lock(&m_enter);
    printf("Adult go out");
    n_adults--;
    pthread_mutex_unlock(&m_enter);
  }

}

int main(int argc, char* argv[]){

  pthread_t threads [MAX_ADULT + MAX_CHILD];
  int i;
  

  srand(time(NULL));
  pthread_mutex_init(&m_adult,NULL);
  pthread_mutex_init(&m_child,NULL);
  pthread_mutex_init(&m_enter, NULL);
  
  sem_init(&sem, 0, 0);

  for(i=0;i<MAX_ADULT;i++)
    if((pthread_create(threads + i, NULL,adult,(void *) i))==-1){
      perror("Tworzenie watku");
      exit(EXIT_FAILURE);
    }
  for(i=0;i<MAX_CHILD;i++)
    if((pthread_create(threads + MAX_ADULT + i, NULL,child, (void *) (MAX_ADULT + i)))==-1){
      perror("Tworzenie watku");
      exit(EXIT_FAILURE);
    }

  for(i=0; i<MAX_ADULT+MAX_CHILD;i++)
    if((pthread_join(threads[i],NULL))==-1){
      perror("Uruchamianie watku");
      exit(EXIT_FAILURE);
    }


 
  return 0;
}
