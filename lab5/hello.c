#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* ----------------------------------------------------------*/
     
void *PrintHello(void *arg);
/*----------------------------------------------------------*/
     
int main(int argc, char *argv[]){
  pthread_t thread;
  int i;
  int rc;
  int *return_value[10];
  int err;
  for(i=0;i<10;i++){
    rc = pthread_create(&thread, NULL, PrintHello, (void *)&i); 
   err = pthread_join(thread,(void**)&(return_value[i]));
    if (rc){
      printf("Return code: %d\n", rc);
      exit(-1);
    }
  }
  
  
  printf("End of the main thread!\n");
  return 0;
}
/* ----------------------------------------------------------*/
     
void *PrintHello(void *arg){
  sleep(1);
  printf("Next boring 'Hello World!' version %d !\n",*( (int*) arg));
  return NULL;
}

