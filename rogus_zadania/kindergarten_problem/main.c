#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CHILD 30
#define MAX_ADULT 10

int n_adults=0, n_childs=0;

pthread_mutex_t m_adult, m_child;

void* child(void* arg){
  int in=0;
  int x;
  while(1){
    x = rand()%2;
    switch(x){
    case(0):
      pthread_mutex_lock(&m_child);
      if(in == 0 && n_childs < MAX_CHILD && n_childs+1 <= n_adults*3){
	n_childs++;
	in++;
	printf("Child come in\n");
      }
      pthread_mutex_unlock(&m_child);
	 break;
    case(1):
      pthread_mutex_lock(&m_child);
      if(n_childs > 0 && in == 1){
	n_childs--;
	in--;
      printf("Child go out\n");
      }
       pthread_mutex_unlock(&m_child);
      break;
     
    }

    
    
    sleep(2);
  }

}

void* adult(void* arg){
  int in=0;
  int x;
  while(1){
    pthread_mutex_lock(&m_adult);
    
    x = rand()%2;
    switch(x){
    case(0):
      if(in == 0 && n_adults < MAX_ADULT){
	printf("adult come in\n");
	n_adults++;
	in++;
      }
      break;
     
    case(1):
      if(in==1 && n_adults >0 && n_childs == 0){
	n_adults--;
	in--;
	printf("adult go out\n");
      }
      else if(in==1 && n_adults >1 && n_childs <= 3*(n_adults-1)){
	n_adults--;
	in--;
	printf("adult go out\n");
      }
      
      break;
	  
    }
    pthread_mutex_unlock(&m_adult);
    printf("Dzieci: %d, Opiekunowie %d\n", n_childs , n_adults);
    sleep(2);

  }

}

int main(int argc, char* argv[]){

  pthread_t threads [MAX_ADULT + MAX_CHILD];
  int i;

  srand(time(NULL));
  pthread_mutex_init(&m_adult,NULL);
  pthread_mutex_init(&m_child,NULL);
  
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
