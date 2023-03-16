#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<unistd.h>

int main() {
	int var;
	int fd = open("entrada.dat", O_RDONLY);
	dup2(fd, 0);
	close(fd);
	scanf("%i", &var);
	printf("var = %d\n", var);
	return 0;
}
