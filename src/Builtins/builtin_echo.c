/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:04 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/21 19:35:51 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
    echo: echo with the -n option requires 0 or more arguments 
	(including the -n option itself and any text to be printed).
        Example: echo hello, echo -n hello world
*/

int	builtin_echo(t_cmd *cmd)
{
	int i;
	int	j;
	int nl;
	char **args;
	int first_word;
	
	i = 1; // Start from 1 to skip the command name
	nl = TRUE; // bash default behavior is to print newline
	args = cmd->args;
	first_word = TRUE;

	// If no arguments are provided, print a newline
	if (!args || !args[0])
	{
		write(1, "\n", 1); // Print newline
		return (SUCCESS); // If no arguments, just print newline
	}

	// BASH HANDLES MULTIPLE '-n', SO FOR EXAMPLE:
	// echo -n -n -n hello world
	// will print "hello world" without a newline at the end.	

	// is first argument -n, if so, do not print newline
	//
	// echo -nnn hello
	j = 1;
	while (args[1][0] == '-' && args[1][j] == 'n') 
	{
		nl = FALSE; // Set nl to FALSE to skip newline
		if (args[1][j + 1] == '\0')
		{
			i++;
			break;
		}
		j++;
	}

	// HANDLE INFILE AND OUTFILE REDIRECTS
	// If infile or outfile are set, we should not print anything
	// to stdout, but redirect the output to the specified file.
	// >, <, >>, <<.
	// Print arguments
	while (args[i] != NULL)
	{
		if (!first_word)
			write(1, " ", 1);
		write(1, args[i], strlen(args[i]));
		//printf("%s\n", args[i]);
		first_word = FALSE;
		i++;
	}
	if (nl == TRUE)
		write(1, "\n", 1);

	return (SUCCESS);
}
