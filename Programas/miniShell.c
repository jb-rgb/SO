#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define MAX_ARGS 64
#define MAX_LINE_LENGTH 1024

int main() {
    char input[MAX_LINE_LENGTH];
    char *args[MAX_ARGS];
    char *arg;
    pid_t pid;
    int status;
    int i, j;
    int pipefd[2];
    int input_redirect = 0;
    int output_redirect = 0;
    char *input_file = NULL;
    char *output_file = NULL;

    while (1) {
        // Mostrar el prompt y leer la entrada del usuario
        printf("> ");
        fflush(stdout);
        fgets(input, MAX_LINE_LENGTH, stdin);

        // Eliminar el carácter de nueva línea final
        input[strcspn(input, "\n")] = 0;

        // Salir del bucle si el usuario ingresa "exit"
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // Analizar la línea de entrada en argumentos
        i = 0;
        arg = strtok(input, " ");
        while (arg != NULL) {
            if (strcmp(arg, "<") == 0) {
                // Redirigir la entrada desde un archivo
                input_redirect = 1;
                arg = strtok(NULL, " ");
                input_file = arg;
                arg = strtok(NULL, " ");
            } else if (strcmp(arg, ">") == 0) {
                // Redirigir la salida hacia un archivo
                output_redirect = 1;
                arg = strtok(NULL, " ");
                output_file = arg;
                arg = strtok(NULL, " ");
            } else if (strcmp(arg, "|") == 0) {
                // Ejecutar dos comandos separados por una tubería
                arg = strtok(NULL, " ");

                // Crear la tubería
                if (pipe(pipefd) == -1) {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }

                // Crear el primer proceso hijo
                pid = fork();
                if (pid == -1) {
                    perror("fork");
                    exit(EXIT_FAILURE);
                } else if (pid == 0) {
                    // El primer proceso hijo ejecuta el primer comando y escribe su salida en la tubería
                    close(pipefd[0]);               // Cierra el extremo de lectura de la tubería
                    dup2(pipefd[1], STDOUT_FILENO); // Redirige la salida estándar al extremo de escritura de la tubería
                    close(pipefd[1]);               // Cierra el extremo de escritura de la tubería

                    // Ejecuta el primer comando
                    args[i] = NULL;
                    if (input_redirect) {
                        freopen(input_file, "r", stdin);
                    }
                    if (output_redirect) {
                        freopen(output_file, "w", stdout);
                    }
                    execvp(args[0], args);
                    perror(args[0]);
                    exit(EXIT_FAILURE);
                }

                // Crear el segundo proceso hijo
                pid = fork();
                if (pid == -1) {
                    perror("fork");
                    exit(EXIT_FAILURE);
                } else if (pid == 0) {
                    // El segundo proceso hijo ejecuta el segundo comando y lee su entrada desde la tubería
                    close(pipefd[1]);              // Cierra el extremo de escritura de la tubería
                    dup2(pipefd[0], STDIN_FILENO); // Redirige la entrada estándar
                    close(pipefd[0]);              // Cierra el extremo de lectura de la tubería

                    // Ejecuta el segundo comando
                    args[i] = arg;
                    j = i;
                    while (args[j] != NULL) {
                        j++;
                        arg = strtok(NULL, " ");
                        args[j] = arg;
                    }
                    if (input_redirect) {
                        freopen(input_file, "r", stdin);
                    }
                    if (output_redirect) {
                        freopen(output_file, "w", stdout);
                    }
                    execvp(args[i], &args[i]);
                    perror(args[i]);
                    exit(EXIT_FAILURE);
                }

                // Espera a que los dos procesos hijos terminen
                close(pipefd[0]);
                close(pipefd[1]);
                waitpid(-1, &status, 0);
                waitpid(-1, &status, 0);

                // Restablecer las variables de redirección
                input_redirect = 0;
                output_redirect = 0;
                input_file = NULL;
                output_file = NULL;

                // Leer el siguiente argumento
                arg = strtok(NULL, " ");
            } else {
                // Añadir el argumento a la lista de argumentos
                args[i] = arg;
                i++;
                arg = strtok(NULL, " ");
            }
        }
        args[i] = NULL;

        // Crear un proceso hijo para ejecutar el comando
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // El proceso hijo ejecuta el comando
            if (input_redirect) {
                freopen(input_file, "r", stdin);
            }
            if (output_redirect) {
                freopen(output_file, "w", stdout);
            }
            execvp(args[0], args);
            perror(args[0]);
            exit(EXIT_FAILURE);
        } else {
            // El proceso padre espera a que el hijo termine
            waitpid(pid, &status, 0);

            // Restablecer las variables de redirección
            input_redirect = 0;
            output_redirect = 0;
            input_file = NULL;
            output_file = NULL;
        }
    }
    return 0;
}