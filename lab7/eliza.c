#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fifo.h"
#include <errno.h>
#include <fcntl.h>
#define MAX 100
#define SHORT 10
#define VERYSHORT 2



int main (void)
{
  struct message msg;
  char* fifosrvbasename = "srvfifoqueue";
  char fifosrvname[FIFO_NAME_BUF_SIZE];
  char fifocntname[FIFO_NAME_BUF_SIZE];
  char s[80];
  int fdsrv,fdcnt,bread=0,bwrite;

  printf("Client Eliza started...\n");
  setbuf(stdout, NULL);
  msg.pid = getpid();

  /* make_cnt_fifo_queue_name(fifocntname, msg.pid, FIFO_NAME_BUF_SIZE); */
/*   printf("Creating client fifo queue \'%s\'...", fifocntname); */
/*   if((mkfifo(fifocntname, PERM_FILE) == -1) && (errno != EEXIST)) */
/*     { */
/*       printf("FAIL!\nError: %s\n", strerror(errno)); */
/*       return 0; */
/*     } */

/*   printf("OK\nOpening server fifo queue \'%s\' for writing...", fifosrvname); */
make_srv_fifo_queue_name(fifosrvname, fifosrvbasename, FIFO_NAME_BUF_SIZE);
  fdsrv = open(fifosrvname, O_WRONLY);
 if(fdsrv == -1)
    {
      printf("FAIL!\nError: %s\n", strerror(errno));
      return 0;
    }
  printf("OK\n");

  /* otwieranie do czytania */
/* if((mkfifo("fifo_2", PERM_FILE) == -1) && (errno != EEXIST)) */
/*     { */
/*       printf("FAIL!\nError: %s\n", strerror(errno)); */
/*       return 0; */
/*     } */

  
  fdcnt = open("fifo_2", O_RDONLY);
  if(fdcnt == -1)
    {
      printf("FAIL!\nError: %s\n", strerror(errno));
      return 0;
    }



  /* printf ("%s\n",response[res++]); */
  do {
    bread = -1;
    bwrite = -1;
    printf ("> ");
    /* p_pos=s */
    if(stdin_readall(msg.data, MESSAGE_BUF_SIZE) == 0)
      break;
    /* gets(s); */
    printf("Writing message to server...\n");
    bwrite = write(fdsrv, &msg, sizeof(msg));
    if(bwrite == -1)
      {
	printf("FAIL!\nError: %s\n", strerror(errno));
	break;
      }
    /* respond (s); */
    printf("czekam na odpowiedz: poslano %s %d\n", msg.data, bwrite);
    bread = read(fdcnt, &msg, sizeof(msg));
      if(bread == -1)
	{
	  printf("FAIL!\nError: %s\n", strerror(errno));
	  close(fdcnt);
	  break;
	}
      /* close(fdcnt); */

      printf("%s\n", msg.data);

  } while (strcmp(s,"bye"));
  return 0;
}


