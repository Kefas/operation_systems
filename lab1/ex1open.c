#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

/*dołaczenie biblioteki, pazurki " " oznaczaja ze biblioteka nie jest biblioteka
systemowa tylko nasza wlasna znajdujacja sie w tym pliku */
#include "t.h"
 
#define BUFSIZE 1024
 
int main (int argc, char **argv) {
  int f1, c;
    char b[BUFSIZE], *n1;
 
    ssize_t  w_read;
    c = 10;
    n1 = argv[1];

    timestart();

    f1 = open (n1, O_RDONLY);
    if(f1 == -1){
      perror("Blad funkcji open");
      exit(EXIT_FAILURE);
    }
   w_read =  read (f1, b, c);
   if(w_read == -1){
     perror("Blad funkcji read");
     exit(EXIT_FAILURE);
   }

    printf("%s: Przeczytano %d znakow z pliku %s: \"%s\"\n",
	   argv[0], c, n1, b);
    close(f1);

    timestop("end");

    return(0);
}
