#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(void)
{
	int fdin, fdout, liczba, i, wynik;
	
	if (mkfifo("/tmp/pierw1", 0666) < 0 ||
		mkfifo("/tmp/pierw2", 0666) < 0)
	{
		perror("Nie udalo sie utworzenie kolejek");
		return 1;
	}
	fdin = open("/tmp/pierw1", O_RDONLY);
	fdout = open("/tmp/pierw2", O_WRONLY);
	
	if (fdin < 0 || fdout < 0)
	{
		perror("Nie moge otworzyc kolejek");
		return 1;
	}
	printf("Kolejki gotowe.\n");
	fflush(stdout);
	read(fdin, &liczba, sizeof(int));
	printf("Sprawdzam %d\n", liczba);
	wynik = 0;
	for (i = 2; i <= liczba / 2 + 1; i++)
		if (liczba % i == 0)
		{
			wynik = 1;
			break;
		}
	write(fdout, &wynik, sizeof(int));
	close(fdin);
	close(fdout);
	return 0;	
}
