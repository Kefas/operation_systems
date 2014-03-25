#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 512


int main(int argc, char** argv){
  int arg = 0;
  int flags = 0;
  int openfd;
  int nread = 0;
  int i,j,counter=0;
  char buffor[BUFFSIZE] = {0};
  char c;
  int n_flag = 0;

  for(i = 1; i < argc; i++){
    if(argv[i][0] == '-'){
      flags++;
      if(argv[i][1] == 'n')
	n_flag = 1;
    }else
      arg++;
  }
  if( arg  > 0 ){
    
    for(i=1;i<=arg;i++){
      if( (openfd = open(argv[i], O_RDONLY, S_IRUSR, S_IRGRP, S_IROTH)) == -1){
	perror(argv[1]);
	exit(EXIT_FAILURE);
      }
      while ((nread = read(openfd, buffor, sizeof(buffor))) > 0){
	/* printf("%s\n", buffor); */
	c = buffor[0];
	j = 0;

	while(c != '\0' ){
	  if(j==0 && n_flag)
	    printf("0. ");
	  printf("%c",c);
	  if(c=='\n' && n_flag)
	    printf("%d. ", ++counter);
	  c= buffor[++j];
	}
      }
    }
  }
  else{
    /*przepisywanie z wejscia */
    while((nread = read(0, buffor, sizeof(buffor))) > 0){
      /* printf("%s \n", buffor); */ 
      j = 0;
      fflush(stdout);
      fflush(stdin);
      
      do{
      	c= buffor[j++];
	printf("%c",c);
      }while(c != '\n');
    }
  }

  return 0;
}
