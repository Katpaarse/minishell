/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:09 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/29 15:40:48 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
    exit: exit with no options requires 0 arguments.
        Example: exit
*/

int	builtin_exit(t_cmd *cmd, t_minishell *shell)
{
	int		i;
	long	exit_code;

	write(1, "exit\n", 5);

	if (cmd->args[1] && cmd->args[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (FAILURE);
	}

	if (!cmd->args[1])
	{
		// free resources if needed
		exit(shell->exit_code); // Exit with the last command exit code
	}

	i = 0;
	while (cmd->args[1][i] != '\0')
	{
		if (!ft_isdigit(cmd->args[1][i])) // '-' and '+' DONT FORGET
		{
			write(2, "minishell: exit: ", 17);
			write(2, cmd->args[1], ft_strlen(cmd->args[1]));
			write(2, ": numeric argument required\n", 28);
			// free resources if needed
			exit(2); // exit with status 2 for non-numeric argument
		}
		i++;
	}

	exit_code = ft_atol(cmd->args[1]);

	exit(exit_code % 256); // Exit with the provided exit code modulo 256
}
