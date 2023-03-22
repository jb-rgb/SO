#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<fcntl.h>

int main() {
	int fd[2];
	pid_t pid;
	pipe(fd);
	pid = fork();

	if(pid == 0) {
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		execl("/bin/ls", "/bin/ls", "-lh", NULL);
		// printf("Hola Mundo\n");
	}
	pid = fork();
	if(pid == 0) {
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		execl("/usr/bin/wc", "/usr/bin/wc", NULL);
		// printf("Hola Mundo\n");
	}
	return 0;
}
