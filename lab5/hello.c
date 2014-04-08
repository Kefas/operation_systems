#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* ----------------------------------------------------------*/
     
void *PrintHello(void *arg);
/*----------------------------------------------------------*/
     
int main(int argc, char *argv[]){
  pthread_t thread[10];
  int i;
  int rc;
  int *return_value[10];
  int err;
  for(i=0;i<10;i++){
    rc = pthread_create(&thread[i], NULL, PrintHello, (void *)i); 
    if (rc){
      printf("Return code: %d\n", rc);
      exit(-1);
    }
  }
  for(i=0;i<10;i++){
    pthread_join(thread[i], NULL);

  }

  
  
  printf("End of the main thread!\n");
  return 0;
}
/* ----------------------------------------------------------*/
     
void *PrintHello(void *arg){
  sleep(1);
  printf("Next boring 'Hello World!' version %d !\n",( (int*) arg));
  return NULL;
}

