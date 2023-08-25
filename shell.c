#include "shell.h"

/**
 * run_shell - start the simple shell.
 */
void run_shell(void)
{
	int status = 0;
	int arg_count = 0;
	int i;
	char *args[MAX_ARGS];
	char *input = NULL;
	char *token;
	size_t buf = 0;
	ssize_t get;

	signal(SIGINT, SIG_IGN);

	do {
		if (isatty(STDIN_FILENO))
    	{
			write(STDOUT_FILENO, "$ ", 2);
		}
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

		if (arg_count > 0) {
			if (strcmp(args[0], "exit") == 0)
			{
				free(input);
				for (i = 0; i < arg_count; i++)
				{
					free(args[i]);
				}
				exit(0);
			}

			handle_command(args);
		}

		for (i = 0; i < arg_count; i++)
		{
			free(args[i]);
		}

	} while (status == 0);

	free(input);
}
