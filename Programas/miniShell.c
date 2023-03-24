#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ARGS 64

int main() {
    char input[MAX_LINE_LENGTH];
    char* args[MAX_ARGS];
    char* arg;
    pid_t pid;
    int status;
    int i, j;
    int pipefd[2];

    while (1) {
        // imprimir el prompt y leer la entrada del usuario
        printf("> ");
        fflush(stdout);
        fgets(input, MAX_LINE_LENGTH, stdin);

        // eliminar el carácter de nueva línea final
        input[strcspn(input, "\n")] = 0;

        // salir del bucle si el usuario ingresa "exit"
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // analizar la línea de entrada en argumentos
        i = 0;
        arg = strtok(input, " ");
        while (arg != NULL) {
            args[i++] = arg;
            arg = strtok(NULL, " ");
        }
        args[i] = NULL;

        // buscar la tubería de comandos
        j = 0;
        while (args[j] != NULL) {
            if (strcmp(args[j], "|") == 0) {
                // dividir los argumentos en dos listas separadas
                args[j] = NULL;
                char** args2 = &args[j+1];

                // crear la tubería
                pipe(pipefd);

                // crear un proceso hijo para ejecutar el segundo comando
                pid = fork();
                if (pid == 0) {
                    // cerrar la entrada estándar y redirigir la entrada desde la tubería
                    close(STDIN_FILENO);
                    dup(pipefd[0]);
                    close(pipefd[0]);
                    close(pipefd[1]);

                    // ejecutar el segundo comando
                    execvp(args2[0], args2);
                    perror("execvp");
                    exit(EXIT_FAILURE);
                } else if (pid < 0) {
                    perror("fork");
                } else {
                    // cerrar la salida estándar y redirigir la salida hacia la tubería
                    close(STDOUT_FILENO);
                    dup(pipefd[1]);
                    close(pipefd[0]);
                    close(pipefd[1]);

                    // ejecutar el primer comando
                    execvp(args[0], args);
                    perror("execvp");
                    exit(EXIT_FAILURE);
                }
            }
            j++;
        }

        // si no hay tubería, crear un proceso hijo y ejecutar el comando
        pid = fork();
        if (pid == 0) {
            // ejecutar el comando
            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("fork");
        } else {
            // esperar a que el proceso hijo termine
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}
