#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_LINE 1024 // La máxima longitud del comando

// Bandera para esperar los procesos que aun no terminan
int espera = 1;

char *tokenizar(char *entrada);
void redireccion_salida(char *Archivo_nombre);
void redireccion_entrada(char *Archivo_nombre);
void crear_Tuberia(char *args[]);

int main(void)
{
    char *args[MAX_LINE]; // Argumentos de línea de comando
    
    while (1) {
        printf("$ ");
        fflush(stdout);


        char prompt[MAX_LINE];
        fgets(prompt, MAX_LINE, stdin);
        char *tokens;
        tokens = tokenizar(prompt);
         // Salir del bucle si el usuario ingresa "exit"
        if (strcmp(tokens, "exit") == 0) {
            break;
        }
        int a;
        //printf("1%s",tokens);
        char *arg = strtok(tokens, " ");
        //printf("2 %s",arg);
        int i = 0;
        while (arg != NULL) {
            if (strcmp(arg, "<") == 0) {
                // printf("ENtrada \n");
                redireccion_entrada(strtok(NULL, " "));// Con strtok se le asigna el nombre del archivo, por que se toma el siguiente parametro
            } else if (strcmp(arg, ">") == 0) {
                // printf("salida \n");
                redireccion_salida(strtok(NULL, " "));// Con strtok se le asigna el nombre del archivo, por que se toma el siguiente parametro
            } else if (strcmp(arg, "|") == 0) {
                // printf("Tuberia \n");
                args[i] = NULL;
                crear_Tuberia(args);
                i = 0;// Iniciar desde 0 cuando se encuentra una tuberia , para que despues se ejecuten los comandos correspondientes 
            } else {
                args[i] = arg;// El comando se va a agregar al arreglo de comandos 
                i++;// Se incrementa la posicion 
            }
            arg = strtok(NULL, " ");
        }
        args[i] = NULL;
  // Ejecutar el proceso si no hay | , < , > 
          pid_t pid;
    
            pid = fork();
            if (pid == -1) { 
                perror("fork");
                exit(EXIT_FAILURE);
            }
            else if ( pid == 0) { // Se crea el proceso hijo
                execvp(args[0], args);
                perror(args[0]);
                exit(EXIT_FAILURE);
            }
            else {//S i no es el proceso padre
                if (espera) {
                    waitpid(pid, NULL, 0);
                } else {
                    espera = 0;
                }
            }
            redireccion_entrada("/dev/tty");
            redireccion_salida("/dev/tty");
    }
    return 0;
}

void redireccion_entrada(char *Archivo_nombre) {
    int entrada = open(Archivo_nombre, O_RDONLY);
    dup2(entrada, 0);
    close(entrada);
}

void redireccion_salida(char *Archivo_nombre) {
    int salida = open(Archivo_nombre, O_WRONLY | O_TRUNC | O_CREAT, 0600);
    dup2(salida, 1);
    close(salida);
}

void crear_Tuberia(char *args[]) {
    int fd[2];
    pipe(fd);

    dup2(fd[1], 1);
    close(fd[1]);
    pid_t pid;
    
    pid = fork();
        if (pid == -1) { 
        perror("fork");
        exit(EXIT_FAILURE);
    }else if ( pid == 0) { /* Proceso hijo */
        execvp(args[0], args);
        perror(args[0]);
        exit(EXIT_FAILURE);
    }
    else { /* Proceso padre */
        if (espera) {
            waitpid(pid, NULL, 0);
        } else {
            espera = 0;
        }
    }
    redireccion_entrada("/dev/tty");
    redireccion_salida("/dev/tty");

    dup2(fd[0], 0);
    close(fd[0]);
}

char *tokenizar(char *entrada) {
    int i;
    int j = 0;
    char *tokenizado = (char *)malloc((MAX_LINE * 2) * sizeof(char));

    // Agregar espacios alrededor de los caracteres especiales
    for (i = 0; i < strlen(entrada); i++) {
        if (entrada[i] != '>' && entrada[i] != '<' && entrada[i] != '|') {
            tokenizado[j++] = entrada[i];
        } else {
            tokenizado[j++] = ' ';
            tokenizado[j++] = entrada[i];
            tokenizado[j++] = ' ';
        }
    }
    tokenizado[j++] = '\0';

    // Poner el caracter null al final del arreglo 
    char *end;
    end = tokenizado + strlen(tokenizado) - 1;
    end--;
    *(end + 1) = '\0';
    
    return tokenizado;
}