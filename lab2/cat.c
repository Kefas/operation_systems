#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFSIZE 2048


int main(int argc, char** argv){
  int arg = 0;
  int flags = 0;
  int openfd;
  int nread = 0;
  int i;
  char buffor[BUFFSIZE];


  for(i = 1; i < argc; i++){
    if(argv[i][0] == '-')
      flags++;
    else
      arg++;
  }
  if(arg > 1){
    fprintf(stderr, "usage: %s no pathname or one pathname\n", argv[0]); 
    return -1;
  }
  else if( arg == 1 ){
    if( (openfd = open(argv[1], O_RDONLY, S_IRUSR, S_IRGRP, S_IROTH)) == -1){
      perror(argv[1]);
      exit(EXIT_FAILURE);
    }
    
    while ((nread = read(openfd, buffor, sizeof(buffor))) > 0){
      printf("%s\n", buffor);
    }
    


  }
  else{


  }

  return 0;
}
