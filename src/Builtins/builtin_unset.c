/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:17 by lavan-de          #+#    #+#             */
/*   Updated: 2025/10/23 15:11:11 by jukerste         ###   ########.fr       */
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
	char	**list;

	if (!cmd || !cmd->args || !shell)
		return (FAILURE);
	i = 1;
	while (cmd->args[i] != NULL)
	{
		if (cmd->args[i][0] == '\0')
		{
			i++;
			continue;
		}
		var_len = 0;
		while (cmd->args[i][var_len] != '=' && cmd->args[i][var_len] != '\0')
			var_len++;
		// Remove from envp
		list = shell->envp;
		j = 0;
		while (list[j] != NULL)
		{
			if (ft_strncmp(list[j], cmd->args[i], var_len) == 0
				&& (list[j][var_len] == '=' || list[j][var_len] == '\0'))
			{
				free(list[j]);
				while (list[j + 1] != NULL)
				{
					list[j] = list[j + 1];
					j++;
				}
				list[j] = NULL;
				break;
			}
			else
				j++;
		}
		// Remove from exp_list
		list = shell->exp_list;
		j = 0;
		while (list[j] != NULL)
		{
			if (ft_strncmp(list[j], cmd->args[i], var_len) == 0
				&& (list[j][var_len] == '=' || list[j][var_len] == '\0'))
			{
				free(list[j]);
				while (list[j + 1] != NULL)
				{
					list[j] = list[j + 1];
					j++;
				}
				list[j] = NULL;
				break;
			}
			else
				j++;
		}
		i++;
	}
	return (SUCCESS); // Always return success unless there's a real error
}
