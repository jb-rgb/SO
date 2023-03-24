#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>

int main() {
	char path[1024];
	char cmd[1024];
	pid_t pid;
	while(1) {
		strcpy(path, "/bin/");
		printf("C:> ");
		scanf("%s", cmd);
		strcat(path, cmd);
		printf("%s\n", path);
		pid = fork();
		if(pid == 0) {
			execl(path, cmd, NULL);
			printf("Comando no encontrado\n");
			exit(-1);
		} else {
			wait(NULL);
		}
	}
	return 0;
}
