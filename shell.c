#include "main.h"

#define MAX_ARGS 10

int main(void) {
    int flag = 1, status;
    int arg_count = 0;
    int i;
    char *args[MAX_ARGS];
    char *input = NULL;
    char *token;
    size_t buf = 0;
    ssize_t get;
    pid_t pid;

    signal(SIGINT, SIG_IGN);

    do {
        write(STDOUT_FILENO, "$ ", 2);
        get = getline(&input, &buf, stdin);

        if (get == -1)
        {
            free(input);
            exit(0);
        }

        arg_count = 0;
        for (i = 0; i < MAX_ARGS; i++)
        {
            args[i] = NULL;
        }

        token = strtok(input, " \t\n");

        while (token != NULL && arg_count < MAX_ARGS - 1)
        {
            args[arg_count] = strdup(token);
            arg_count++;
            token = strtok(NULL, " \t\n");
        }
        args[arg_count] = NULL;

        if (arg_count > 0)
        {
            pid = fork();
            if (pid < 0)
            {
                perror("fork failed !");
                exit(1);
            }
            else if (pid == 0)
            {
                execvp(args[0], args);
                perror("execvp failed !");
                exit(1);
            }
            else
            {
                waitpid(pid, &status, 0);
                if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                    fprintf(stderr, "bash: %s: command not found\n", args[0]);
                }
            }
        }

        for (i = 0; i < arg_count; i++)
        {
            free(args[i]);
        }

    } while (flag == 1);

    free(input);

    return 0;
}
