#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define MAX_READ 20
#define MAX_WRIT 4

int counter;
pthread_mutex_t writer_m, reader_m, ochrona;
pthread_mutex_t turn_r, turn_w;
int min_reader=0, min_writer=0;

void* writer(void *arg){
  int count=0;
  while(1){
    
     
    
    pthread_mutex_lock(&writer_m);   
    printf("Writer %d is writing %d\n", (int *) arg, count++);
    sleep(1);

    pthread_mutex_unlock(&writer_m);
    sleep(rand()%10 +5);
  }

}

void* reader(void *arg){
  int count=0;
  while(1){
    
    pthread_mutex_lock(&reader_m);
    
      counter++;
    if(counter == 1){
      pthread_mutex_lock(&writer_m);
      
      printf("zamkniete dla pisarzy\n");
    }
    
    pthread_mutex_unlock(&reader_m);
      

    printf("Reader %d is Reading %d\n", (int *) arg,count++);
    sleep(rand()%2);

    pthread_mutex_lock(&reader_m);
    counter--;
    if(counter == 0){
      
      printf("Otwarte dla pisarzy\n");
      pthread_mutex_unlock(&writer_m);
      
    }
    pthread_mutex_unlock(&reader_m);
    sleep(rand()%4+1);

  }
  

}



int main(int argc, char* argv[]){
  pthread_t readers[MAX_READ];
  pthread_t writers[MAX_WRIT];
  int i =0;
  counter = 0;
  pthread_mutex_init(&writer_m, NULL);
  pthread_mutex_init(&reader_m, NULL);
  pthread_mutex_init(&turn_r, NULL);
  pthread_mutex_init(&turn_w, NULL);
  pthread_mutex_init(&ochrona, NULL);
  srand(time(NULL));
 for(i=0;i<MAX_WRIT;i++)
   if((pthread_create(writers + i, NULL, writer, (void *) i))==-1){
      perror("Tworzenie watku");
      exit(EXIT_FAILURE);
    }

for(i=0;i<MAX_READ;i++)
  if((pthread_create(readers + i, NULL, reader, (void *) i))==-1){
      perror("Tworzenie watku");
      exit(EXIT_FAILURE);
    }

  for(i=0;i<MAX_WRIT;i++)
    if((pthread_join(writers[i],NULL))==-1){
      perror("Uruchamianie watku");
      exit(EXIT_FAILURE);
    }


  
      
  for(i=0;i<MAX_READ;i++)
    if((pthread_join(readers[i],NULL))==-1){
      perror("Uruchamianie watku");
      exit(EXIT_FAILURE);
    }

 

  return 0;
}
