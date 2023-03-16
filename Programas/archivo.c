#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main() {
	int fd = open("arch.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	dup2(fd, 1);
	close(fd);
	printf("Jorge Barahona\n");
	return 0;
}
