#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

void run_command(char *cmd) {
    char *args[MAX_ARGS];
    int i = 0;

    char *arg = strtok(cmd, " ");
    while (arg != NULL && i < MAX_ARGS - 1) {
        args[i++] = arg;
        arg = strtok(NULL, " ");
    }
    args[i] = NULL;

    execvp(args[0], args);
    perror("execvp");
    exit(EXIT_FAILURE);
}

void run_pipeline(char **commands, int n) {
    if (n == 1) {
        run_command(commands[0]);
        return;
    }

    int fd[2];
    pipe(fd);

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        run_command(commands[0]);
    } else if (pid > 0) {
        // Parent process
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        run_pipeline(commands + 1, n - 1);
        wait(NULL);
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

int main() {
    while (1) {
        char input[MAX_CMD_LEN];
        printf("> ");
        fflush(stdout);
        if (fgets(input, MAX_CMD_LEN, stdin) == NULL) {
            break;
        }
        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }

        char *commands[MAX_ARGS];
        int i = 0;

        char *cmd = strtok(input, "|");
        while (cmd != NULL && i < MAX_ARGS - 1) {
            commands[i++] = cmd;
            cmd = strtok(NULL, "|");
        }
        commands[i] = NULL;

        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            run_pipeline(commands, i);
            exit(EXIT_SUCCESS);
        } else if (pid > 0) {
            // Parent process
            wait(NULL);
        } else {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}
