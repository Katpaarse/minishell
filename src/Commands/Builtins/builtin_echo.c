/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jul <jul@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:04 by lavan-de          #+#    #+#             */
/*   Updated: 2025/10/26 14:05:50 by jul              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Prints arguments to stdout, optionally suppressing trailing newline. 
Handles the '-n' flag to control newline output */
int	builtin_echo(t_cmd *cmd)
{
	int	i;
	int	nl;

	if (!cmd->args[1])
	{
		write(1, "\n", 1);
		return (SUCCESS);
	}
	nl = TRUE;
	i = 1;
	i = skip_nl(cmd, i);
	if (i > 1)
		nl = FALSE;
	write_echo(cmd, i);
	if (nl == TRUE)
		write(1, "\n", 1);
	return (SUCCESS);
}

/* Skips '-n' flags to determine if newline should be suppressed.
Iterates over arguments to check for valid '-n' flags (can handle '-nnnn') */
int	skip_nl(t_cmd *cmd, int i)
{
	int	j;

	j = 1;
	while (cmd->args[1][0] == '-' && cmd->args[1][j] == 'n')
	{
		if (cmd->args[1][j + 1] == '\0')
		{
			i++;
			break ;
		}
		j++;
	}
	return (i);
}

/* Writes arguments to stdout with space separation. Prints each argument
followed by a space, except for the last one */
void	write_echo(t_cmd *cmd, int i)
{
	int	first_word;

	first_word = TRUE;
	while (cmd->args[i] != NULL)
	{
		if (!first_word)
			write(1, " ", 1);
		write(1, cmd->args[i], strlen(cmd->args[i]));
		first_word = FALSE;
		i++;
	}
}
