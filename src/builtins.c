/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 18:21:49 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/13 18:22:09 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
Summary Table
Builtin		Uses infile		Uses outfile/append
echo		❌				✅
cd			❌				❌
pwd			❌				✅
export		❌				⚠ optional (rare)
unset		❌				❌
env			❌				✅
exit		❌				❌
*/

int main(void)
{
    t_cmd cmd;
    t_minishell shell;
/*
    cmd.outfile = NULL;
    cmd.append = FALSE;
    cmd.args = NULL; // unused by pwd
    builtin_pwd(&cmd, &shell);

    cmd.outfile = "pwd_output.txt";
    cmd.append = FALSE;
    builtin_pwd(&cmd, &shell);
    printf("'pwd_output.txt' for output\n");
*/

    // echo hello world
    char *args1[] = {"hello", "world", NULL};
    cmd.args = args1;
    cmd.outfile = NULL;
    cmd.append = FALSE;
    builtin_echo(&cmd, &shell);
/*
    // echo -n hello world
    char *args2[] = {"-n", "hello", "world", NULL};
    cmd.args = args2;
    builtin_echo(&cmd, &shell);
*/
    // echo hello world > echo_output.txt
    cmd.outfile = "echo_output.txt";
    cmd.append = FALSE;
    builtin_echo(&cmd, &shell);
    printf("Check file 'echo_output.txt' for output\n");

    return (0);
}

int	builtin_echo(t_cmd *cmd, t_minishell *shell)
{
	int	fd;
	int i;
	int nl;
	char **args;
	int first_word;
	(void)shell;
	

	i = 0;
	nl = TRUE; // Default behavior is to print newline
	args = cmd->args;
	first_word = TRUE;

	// If no arguments are provided, print a newline
	if (!args || !args[0])
	{
		printf("\n");
		printf("Echo command executed\n");
		return (SUCCESS); // If no arguments, just print newline
	}


	// BASH HANDLES MULTIPLE '-n', SO FOR EXAMPLE:
	// echo -n -n -n hello world
	// will print "hello world" without a newline at the end.	

	// is first argument -n, if so, do not print newline
	if (args[0][0] == '-' && args[0][1] == 'n' && args[0][2] == '\0')
	{
		nl = FALSE; // Set nl to TRUE to print newline
		i++;
	}


	// HANDLE INFILE AND OUTFILE REDIRECTS
	// If infile or outfile are set, we should not print anything
	// to stdout, but rather redirect the output to the specified file.
	// >, <, >>, <<.

	/*
	int fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);

	printf("Hello, world!\n");    goes into out.txt
	*/


	// Print arguments
	if (cmd->append == FALSE) // '>' REDIRECT mode
	{
		if (cmd->outfile)
		{
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			if (fd < 0)
			{
				perror("Error opening outfile");
				return (FAILURE);
			}
			// dup2 Redirect stdout to outfile
			close(fd);
		}

		// Print arguments
		while (args[i] != NULL)
		{
			if (!first_word)
				printf(" ");
			printf("%s", args[i]);
			first_word = FALSE;
			i++;
		}
		if (nl == TRUE)
			printf("\n");
	}
	else if (cmd->append == TRUE) // '>>' APPEND mode
	{
		if (cmd->outfile)
		{
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0666);
			if (fd < 0)
			{
				perror("Error opening outfile");
				return (FAILURE);
			}
			// dup2 Redirect stdout to outfile
			close(fd);
		}

		while (args[i] != NULL)
		{
			if (!first_word)
				printf(" ");
			printf("%s", args[i]);
			first_word = FALSE;
			i++;
		}
		if (nl == TRUE)
			printf("\n");
	}

	printf("Echo command executed\n");
	return (SUCCESS);
}



int	builtin_pwd(t_cmd *cmd, t_minishell *shell)
{
	char *cwd;

	(void)cmd; // Unused parameter, can be removed if not needed
	(void)shell; // Unused parameter, can be removed if not needed

/* 
	Get the pathname of the current working directory,
		extern char *getcwd (char *__buf, size_t __size) __THROW __wur;

	Returns NULL if the directory couldn't be determined or SIZE was too small.
	If successful, returns BUF.  In GNU, if BUF is NULL, an array is allocated with `malloc';
	the array is SIZE bytes long, unless SIZE == 0, in which case it is as big as necessary.  
*/
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd() error");
		return (FAILURE);
	}

	if (cmd->outfile)
	{
		int fd;
		int len;
	
		if (cmd->append)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0666); // Open in append mode
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666); // Open in truncate mode
		if (fd < 0)
		{
			perror("Error opening outfile");
			free(cwd);
			return (FAILURE);
		}
		len = strlen(cwd);
		if (write(fd, cwd, len) < 0)
		{
			perror("Error writing to outfile");
			free(cwd);
		}
		if (write(fd, "\n", 1) < 0)
		{
			perror("Error writing newline to outfile");
			free(cwd);
		}
		free(cwd); // Free the allocated memory for cwd
		if (close(fd) < 0)
		{
			perror("Error closing outfile");
			return (FAILURE);
		}
		return (SUCCESS);
	}
	else
	{
		printf("%s\n", cwd);
		if (cwd)
			free(cwd); // Free the allocated memory for cwd
		return (SUCCESS);
	}
}



int	builtin_exit(t_cmd *cmd, t_minishell *shell)
{
	// This function should implement the exit command
	// For now, we will just print a placeholder message
	printf("Exit command executed\n");
	return (SUCCESS);
}
