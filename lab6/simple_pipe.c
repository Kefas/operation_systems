#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/wait.h>

#include <stdlib.h>
 

     
int main(int argc, char *argv[]){
     
  int pfd[2];
  size_t nread;
  char buf[100];
  pid_t cpid;
 
  /*pipe(pfd);
    char * str ="Ur beautiful pipe example";
    write(pfd[1], str, strlen(str));
    nread=read(pfd[0],buf, sizeof(buf));
     
    (nread!=0)?printf("%s (%ld bytes)\n",buf,(long)nread):printf("No data\n");
     
    return 0;
  */

  /* --- 3 --- */
    
  char * str ="Ur beautiful pipe example";

    
  if(pipe(pfd) == -1){
    perror("pipe failure");
    exit(EXIT_FAILURE);
  }
    
  cpid = fork();
    
    
  if(cpid > 0){
    close(pfd[0]);
    write(pfd[1], str, strlen(str));
    close(pfd[1]);
    wait(NULL);
    _exit(EXIT_SUCCESS);
  }else if(cpid == 0){
    close(pfd[1]);
    nread=read(pfd[0],buf, sizeof(buf)); 
    (nread!=0)?printf("%s (%ld bytes)\n",buf,(long)nread):printf("No data\n");
    close(pfd[0]);
    _exit(EXIT_SUCCESS);
  }
    
    
}

