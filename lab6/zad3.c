#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char** argv){
  pid_t pid;
  int pfd[2];
  int fd;
  fd = open("dictionary.txt", O_RDONLY);
  char buff[100];
  

  if(pipe(pfd) == -1){
    perror("pipe failure");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if( pid  == -1 ){
    perror("fork failure");
    exit(EXIT_FAILURE);
  }

  if( pid > 0 ){
    pid = fork();
    if( pid  == -1 ){
      perror("fork failure");
      exit(EXIT_FAILURE);
    }
    if(pid > 0){

    }
    else{

    }

  }
  else{


  }


  return 0;
}
