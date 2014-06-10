/*
** client.c -- a stream socket client demo
*/
     
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>

     
#define PORT 58208 // the port client will be connecting to 
     
#define MAXDATASIZE 100 // max number of bytes we can get at once 

#define REC_FILE "odebrany.txt"
     
int main(int argc, char *argv[])
{
  int sockfd, numbytes;  
  char buf[MAXDATASIZE];
  struct hostent *he;
  struct sockaddr_in their_addr; // connector's address information 
  FILE* receive_fd;
  int file_size;
  int remain_data;
  int len;

  if (argc != 2) {
    fprintf(stderr,"usage: client hostname\n");
    exit(1);
  }
     
  if ((he=gethostbyname(argv[1])) == NULL) {  // get the host info 
    perror("gethostbyname");
    exit(1);
  }
     
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }
     
  their_addr.sin_family = AF_INET;    // host byte order 
  their_addr.sin_port = htons(PORT);  // short, network byte order 
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);
  memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);
     
  if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof their_addr) == -1) {
    perror("connect");
    exit(1);
  }
     
  if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
    perror("recv");
    exit(1);
  }
  file_size = atoi(buf);   
  buf[numbytes] = '\0';
     
  printf("Received: %s",buf);

  sleep(1);

  receive_fd = fopen(REC_FILE, "w");
  remain_data = file_size;  
  printf("odbieram cos? \n");
  while (((len = recv(sockfd, buf, BUFSIZ, 0)) > 0) && (remain_data > 0))
    {
      
      fwrite(buf, sizeof(char), len, receive_fd);
      remain_data -= len;
      fprintf(stdout, "Receive %d bytes and we hope :- %d bytes\n", len, remain_data);
    }
  printf("odebrano %d\n", len);
  fclose(receive_fd);
  
  close(sockfd);
     
  return 0;
}

