#include "shell.h"

/**
 * main - Entry point of the shell program.
 *
 * Return: Always 0.
 */
int main(void)
{
	run_shell();
	return (0);
}

/**
 * handle_command - Execute a command with arguments.
 * @args: Array of command arguments.
 */
void handle_command(char *args[])
{
	int status = 0;
	pid_t pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed !");
		exit(1);
	} else if (pid == 0)
	{
		execvp(args[0], args);
		perror(args[0]);
		exit(127);
	} else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			fprintf(stderr, "%s: command not found\n", args[0]);
		}
	}
}