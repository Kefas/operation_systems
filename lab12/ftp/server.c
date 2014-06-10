/*
** server.c -- a stream socket server demo
*/
     
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

     
#define MYPORT 58208	// the port users will be connecting to
     
#define BACKLOG 10	 // how many pending connections queue will hold

#define FILE_TO_SEND "toSend.txt"
     
void sigchld_handler(int s)
{
  while(waitpid(-1, NULL, WNOHANG) > 0);
}
     
int main(void)
{
  int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
  struct sockaddr_in my_addr;	// my address information
  struct sockaddr_in their_addr; // connector's address information
  socklen_t sin_size;
  struct sigaction sa;
  int yes=1;
  int send_fd;
  struct stat file_stat;
  int offset = 0;
  char file_size[10];
  int remain_data;
  int sent_bytes;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }
     
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    exit(1);
  }
     
  my_addr.sin_family = AF_INET;		 // host byte order
  my_addr.sin_port = htons(MYPORT);	 // short, network byte order
  my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
  memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);
     
  if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
    perror("bind");
    exit(1);
  }
     
  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }
     
  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }
     
  sin_size = sizeof their_addr;
  if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
    perror("accept");
  }
  printf("server: got connection from %s\n",inet_ntoa(their_addr.sin_addr));

  if((send_fd = open(FILE_TO_SEND, O_RDONLY)) == -1){
    perror("fopen");
  }
  if(fstat(send_fd, &file_stat) == -1)
    perror("stat");
  sprintf(file_size, "%d", file_stat.st_size);

  if(send(new_fd, file_size, sizeof(file_size),0) == -1)
   perror("send");

  printf("file_size %s\n", file_size);
  offset = 0;
  remain_data = file_stat.st_size;
  /* Sending file data */
  printf("wysylam plik\n");

 
  sleep(2);

  while (((sent_bytes = sendfile(new_fd, send_fd, NULL, BUFSIZ)) > 0) && (remain_data > 0)){
      printf("1. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
      remain_data -= sent_bytes;
      printf("2. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
    }

  close(new_fd);
     
  return 0;
}

