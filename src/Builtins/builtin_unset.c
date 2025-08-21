/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:17 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/21 19:36:18 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
    unset: unset with no options requires 0 arguments.
        Example: unset
*/

int	builtin_unset(t_cmd *cmd, t_minishell *shell)
{
	int i;
	int j;
	int var_len;
	int unset_check;
	(void)shell; // Unused parameter, can be removed if not needed

	if (!cmd || !cmd->args || !shell)
		return (FAILURE);

	var_len = 0;
	unset_check = FAILURE;

	i = 1;
	while (cmd->args[i] != NULL)
	{
		if (cmd->args[i][0] == '\0' && cmd->args[i + 1] != NULL)
		{
			i++;
			continue;
		}
		var_len = 0;
		while (cmd->args[i][var_len] != '=' && cmd->args[i][var_len] != '\0')
			var_len++;

		j = 0;
		while (shell->envp[j] != NULL)
		{
			if (ft_strncmp(shell->envp[j], cmd->args[i], var_len) == 0
				&& (shell->envp[j][var_len] == '=' || shell->envp[j][var_len] == '\0'))
			{
				free(shell->envp[j]);
				// Shift elements to the left
				while (shell->envp[j + 1] != NULL)
				{
					shell->envp[j] = shell->envp[j + 1];
					j++;
				}
				shell->envp[j] = NULL;
				unset_check = SUCCESS; // At least one variable was unset
				i++; // Move to the next argument
				break; // continue to next argument
			}
			else
				j++;
		}
	}
	// If no variables were unset, return FAILURE
	if (unset_check == FAILURE)
	{
		//error
		return (FAILURE);
	}
	return (SUCCESS);
}
