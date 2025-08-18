/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:04 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/18 18:59:44 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
    echo: echo with the -n option requires 0 or more arguments 
	(including the -n option itself and any text to be printed).
        Example: echo hello, echo -n hello world
*/

int	builtin_echo(t_cmd *cmd, t_minishell *shell)
{
	int	fd;
	int i;
	int nl;
	char **args;
	int first_word;
	(void)shell;
	
	i = 1; // Start from 1 to skip the command name
	nl = TRUE; // bash default behavior is to print newline
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
	while (args[i][0] == '-' && args[i][1] == 'n' && args[i][2] == '\0')
	{
		nl = FALSE; // Set nl to TRUE to print newline
		i++;
	}

	// HANDLE INFILE AND OUTFILE REDIRECTS
	// If infile or outfile are set, we should not print anything
	// to stdout, but redirect the output to the specified file.
	// >, <, >>, <<.

	if (cmd->outfile)
	{
		if (cmd->append == TRUE)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0666); // '>>' APPEND mode
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666); // '>' REDIRECT mode

		if (fd < 0)
		{
			perror("Error opening outfile");
			return (FAILURE);
		}
		dup2(fd, STDOUT_FILENO); // Redirect stdout to outfile
		close(fd);

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
	printf("Echo command executed\n");
	return (SUCCESS);
}
