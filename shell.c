#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGS 10

int main(void)
{
    int flag = 1, status;
    ssize_t get;
    size_t buf = 0;
    pid_t pid;
    char* args[MAX_ARGS];
    char *input = NULL;
    int arg_count = 0;
    char *token;

    do {
        write(STDOUT_FILENO, "$ ", 2);
        get = getline(&input, &buf, stdin);
        if (get == -1)
        {
            free(input);
            exit(0);
        }

        arg_count = 0;
        token = strtok(input, " \t\n");
    
        while (token != NULL && arg_count < MAX_ARGS - 1)
        {
            args[arg_count] = token;
            arg_count++;
            token = strtok(NULL, " \t\n");
        }
        args[arg_count] = NULL;

        pid = fork();
        if (pid < 0)
        {
            perror("fork failed");
            exit(1);
        }
        else if (pid == 0)
        {
            execvp(args[0], args);
            perror("execvp failed");
            exit(1);
        }
        else
        {
            waitpid(pid, &status, 0);
        }

    } while (flag == 1);

    free(input);

    return (0);
}
