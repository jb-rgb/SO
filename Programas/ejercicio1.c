// Escriba un programa que genere un proceso. En el proceso que ejecute el comando
// ls y la salida se guarde en un archivo llamado arch.txt

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<unistd.h>

int main() {
	pid_t pid;
	char *argv[] = { "/bin/ls", "-lh", NULL };
	pid = fork();
	if(pid == 0) {
		int fd = open("arch.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600);
		dup2(fd, 1);
		close(fd);
		execv("/bin/ls", argv);
	}
	else {
		wait(NULL);
	}
	return 0;
}
