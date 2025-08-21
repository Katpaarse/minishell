/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:09 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/21 19:36:02 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
    exit: exit with no options requires 0 arguments.
        Example: exit
*/

int	builtin_exit(t_cmd *cmd, t_minishell *shell)
{
	if (cmd->args[2] != NULL)
	{
		write(STDERR_FILENO, "exit\n", 5);
		write(STDERR_FILENO, "minishell: exit: too many arguments\n", 35);
		return (FAILURE);
	}

	// exit with no arguments or with one argument
	if (cmd->args[1] == NULL)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		exit(shell->exit_code); // Exit with the last command exit code
	}
	else if (cmd->args[1] != NULL && cmd->args[2] == NULL)
	{
		shell->exit_code = atoi(cmd->args[1]);

		while (shell->exit_code < 0 || shell->exit_code > 255)
			shell->exit_code = shell->exit_code % 256; // Ensure exit code is within 0-255 range

		write(STDOUT_FILENO, "exit\n", 5);
		exit(shell->exit_code); // Exit with the specified exit code
	}
	return (SUCCESS);
}
