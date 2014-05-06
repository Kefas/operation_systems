#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(void)
{
	int fdin, fdout, liczba, wynik;

	fdout = open("/tmp/pierw1", O_WRONLY);
	fdin = open("/tmp/pierw2", O_RDONLY);
	if (fdin < 0 || fdout < 0)
	{
		perror("Nie moge otworzyc kolejek");
		return 1;
	}
	printf("Kolejki otwarte.\nPodaj liczbe: ");
	scanf("%d", &liczba);
	write(fdout, &liczba, sizeof(int));
	read(fdin, &wynik, sizeof(int));
	if (wynik == 0)
		printf("%d jest liczba pierwsza\n", liczba);
	else
	  printf("%d jest liczba zlozona\n", liczba);
	close(fdout);
	close(fdin);
	unlink("/tmp/pierw1");
	unlink("/tmp/pierw2");
	return 0;	
}
