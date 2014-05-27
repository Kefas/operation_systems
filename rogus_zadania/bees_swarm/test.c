#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h> 


pthread_t* threads;
void* syf(void * arg){
  

}

int main(void){

  
  int i;
  threads = (pthread_t*) malloc( 50 * sizeof(pthread_t));


  for(i=0;i<50;i++){
    pthread_create(threads+i,NULL,syf,(void*)i);
  }
  


  return 0;

}
