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

#define MAX_LINE 1024 // The maximum length command

//int should_run = 1;  // flag to determine when to exit program
int should_wait = 1; // flag to determine if process should run in the background


void redireccion_entrada(char *fileName)
{
    int in = open(fileName, O_RDONLY);
    dup2(in, 0);
    close(in);
}


void redireccion_salida(char *fileName)
{
    int out = open(fileName, O_WRONLY | O_TRUNC | O_CREAT, 0600);
    dup2(out, 1);
    close(out);
}

/* 
void run(char *args[])
{
    pid_t pid;
    
            pid = fork();
            if (pid < 0) { 
                fprintf(stderr, "Fork Failed");
            }
            else if ( pid == 0) { //child process 
                execvp(args[0], args);
            }
            else { // parent process 
                if (should_wait) {
                    waitpid(pid, NULL, 0);
                } else {
                    should_wait = 0;
                }
            }
            redireccion_entrada("/dev/tty");
            redireccion_salida("/dev/tty");
}
*/

void createPipe(char *args[])
{
    int fd[2];
    pipe(fd);

    dup2(fd[1], 1);
    close(fd[1]);

    //printf("args = %s\n", *args);

      pid_t pid;
    
            pid = fork();
            if (pid < 0) { 
                fprintf(stderr, "Fork Failed");
            }
            else if ( pid == 0) { /* child process */
                execvp(args[0], args);
            }
            else { /* parent process */
                if (should_wait) {
                    waitpid(pid, NULL, 0);
                } else {
                    should_wait = 0;
                }
            }
            redireccion_entrada("/dev/tty");
            redireccion_salida("/dev/tty");

    dup2(fd[0], 0);
    close(fd[0]);
}

/**
 * Creates a tokenized form of the input with spaces to separate words.
 * 
 * @param  *input the input string
 * @return tokenized the tokenized stirng
 */
char *tokenize(char *input)
{
    int i, j = 0;
    char *tokenized = malloc(sizeof(char) * (strlen(input) + 1));

    for (i = 0; input[i] != '\0'; i++) {
        if (input[i] == '>' || input[i] == '<' || input[i] == '|') {
            tokenized[j++] = ' ';
            tokenized[j++] = input[i];
            tokenized[j++] = ' ';
        } else {
            tokenized[j++] = input[i];
        }
    }
    tokenized[j] = '\0';

    char *end;
    end = tokenized + strlen(tokenized) - 1;
    end--;
    *(end + 1) = '\0';

    return tokenized;
}

/**
 * Runs a basic shell.
 * 
 * @return 0 upon completion
 */
int main(void)
{
    char *args[MAX_LINE]; // command line arguments
    
    while (1) {
        printf("MillerShell$ ");
        fflush(stdout);

        char input[MAX_LINE];
        fgets(input, MAX_LINE, stdin);

         // Salir del bucle si el usuario ingresa "exit"
      

        char *tokens;
        tokens = tokenize(input);
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
                printf("ENtrada \n");
                redireccion_entrada(strtok(NULL, " "));
            } else if (strcmp(arg, ">") == 0) {
                printf("salida \n");
                redireccion_salida(strtok(NULL, " "));
            } else if (strcmp(arg, "|") == 0) {
                printf("Tuberia \n");
                args[i] = NULL;
                createPipe(args);
                i = 0;
            } else {
                args[i] = arg;
                i++;
            }
            arg = strtok(NULL, " ");
        }
        args[i] = NULL;
  
          pid_t pid;
    
            pid = fork();
            if (pid < 0) { 
                fprintf(stderr, "Fork Failed");
            }
            else if ( pid == 0) { /* child process */
                execvp(args[0], args);
            }
            else { /* parent process */
                if (should_wait) {
                    waitpid(pid, NULL, 0);
                } else {
                    should_wait = 0;
                }
            }
            redireccion_entrada("/dev/tty");
            redireccion_salida("/dev/tty");
    }
    return 0;
}