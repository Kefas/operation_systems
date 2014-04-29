#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
     
int main(void)
{
  int     pfd[2];
  pid_t   pid;
  char    string[] = "Test";
  char    buf[10];
     
  if(pipe(pfd) == -1){
    perror("pipe failure");
    exit(EXIT_FAILURE);
  }

  pid = fork();
     
  if(pid == 0) {
    close(0);
    if(dup2(pfd[0], 0) == -1){
      perror("dup2 failure");
      exit(EXIT_FAILURE);
    }
    
    read(STDIN_FILENO, buf, sizeof(buf));
    close(pfd[0]);
    printf("Wypisuje: %s", buf);
  } else {
    close(pfd[0]);
    write(pfd[1], string, (strlen(string)+1));
    close(pfd[1]);
  }
     
  return 0;
}

/* close(0) jest uzyta do zamkniecia standardowego wejscia, zeby móc mu przypisac nowy proces jako STDIN_FILENO

 */

