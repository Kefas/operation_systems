#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h> 

#define BEES 50
#define MAX_BEES 25
#define LIFE 5


int n_bees=BEES;
int bees_in=0;
int bee_queen=1;
int kill_oldest=0;
int old=5;

pthread_t* threads;
int* urodzona;
size_t size;

sem_t door;
sem_t max_bees;

pthread_mutex_t hive;


void* bee(void* arg){
  int life = LIFE;
  while(1){
    sleep(rand()%10);
    
      

    sem_wait(&max_bees); 
    sem_wait(&door);
    pthread_mutex_lock(&hive);
    bees_in++;
    printf("Bee %d in hive, all in: %d of %d\n", (int)arg, bees_in, n_bees);
    pthread_mutex_unlock(&hive);
    sem_post(&door);
    
    sleep(3);

    sem_wait(&door);
    pthread_mutex_lock(&hive);
    printf("Bee %d is gone, all in: %d of %d\n",(int)arg,bees_in,n_bees);
    bees_in--;
    pthread_mutex_unlock(&hive);
    sem_post(&door);
    sem_post(&max_bees);

    if(--life < 1){
      if(life < old)
	old = life;
      printf("Bee %d is dead\n",(int)arg);
      n_bees--;
      pthread_exit((void*)1);
    }
    if(kill_oldest == 1 && life <= old){
      pthread_mutex_lock(&hive);
      n_bees--;
      pthread_mutex_unlock(&hive);
      kill_oldest = 0;
      printf("Bee %d killed\n", (int) arg);
      pthread_exit((void*)1);
      
    }

  } 


}

void* queen(void* arg){
  int random=0;
  int i;
  while(bee_queen==1){
    sleep(rand()%20);
    if(bee_queen == 0){
      free(threads);
      pthread_exit(1);
    }
    if(MAX_BEES > bees_in)
      random = rand()%(MAX_BEES - bees_in); 
    
    printf("Queen born %d new bees\n", random);
    
    pthread_t* temp = (pthread_t*) malloc((size+random)*sizeof(pthread_t));
    free(threads);
    threads = temp;
 
    for(i=size;i<size+random;i++){
      pthread_mutex_lock(&hive);
      n_bees++;
      pthread_mutex_unlock(&hive);
      pthread_create(threads+i,NULL,bee,(void*)i);
    }

    size += random;


  }

  
 
}



int main(int argc, char* argv[]){
  int i;
  
  char c;

  srand(time(NULL));

  sem_init(&door, 0, 2);
  sem_init(&max_bees,0, MAX_BEES);

  pthread_mutex_init(&hive,0);

  if((threads = (pthread_t*) malloc( (BEES + 1) * sizeof(pthread_t) )) == NULL){
    perror("mallocowanie");
    exit(EXIT_FAILURE);
  }

  urodzona = (int *) malloc( (BEES+1) * sizeof( int));
  for(i = 0;i<BEES+1;i++)
    urodzona[i] = 0;

  size = BEES+1;
 
  
  if((pthread_create(threads,NULL, queen,(void *)0)) == -1){
    perror("thread creation");
    exit(EXIT_FAILURE);
  }
   
  for(i=1;i<BEES+1;i++){
    if((pthread_create(threads+i,NULL,bee,(void *)i)) == -1){
      perror("thread creation");
      exit(EXIT_FAILURE);
    }
  }
 


  while(c != 'q' && (n_bees >5 || bee_queen == 1)){
    printf("wypisz\n");
    
    c = getchar(); 

    switch(c){
    case 'p':
      printf("Killing the oldest bee\n");
      kill_oldest = 1;
      break;
    case 'k':
      printf("Killing queen..\n");
      bee_queen = 0;
      break;
    default:
      printf("Try p or k or q\n");
      break;
    }

  }
  
  



  return 0;
}
