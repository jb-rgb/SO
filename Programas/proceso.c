#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main(){
	pid_t pid;
	char *argv[] = { "/bin/ls", "-lh", NULL };
	char *envp[] = { NULL, NULL, NULL, NULL };
	pid = fork();
	if(pid == 0) {
		// execl("/bin/ls", "/bin/ls", "-lh", NULL);
		// execlp("/bin/ls", "/bin/ls", "-lh", NULL);
		// execle("/bin/ls", "/bin/ls", "-lh", NULL, envp);
		// execv("/bin/ls", argv);
		// execvp("/bin/ls", argv);
		execvpe("/bin/ls", argv, envp);
	}
	else {
		// printf("estamos en el proceso padre");
		wait(NULL);
	}
	return 0;
}

