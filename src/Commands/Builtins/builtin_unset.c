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

void	shift_list(char **list, int index)
{
	free(list[index]);
	while (list[index] != NULL)
	{
		list[index] = list[index + 1];
		index++;
	}
	list[index] = NULL;
}

char	*check_env(t_cmd *cmd, t_minishell *shell, int var_len, int i)
{
	int		j;
	char	**env_list;

	while (cmd->args[i][var_len] != '=' && cmd->args[i][var_len] != '\0')
		var_len++;
	env_list = shell->envp;
	j = 0;
	while (env_list[j] != NULL)
	{
		if (ft_strncmp(env_list[j], cmd->args[i], var_len) == 0
			&& (env_list[j][var_len] == '=' || env_list[j][var_len] == '\0'))
		{
			shift_list(env_list, j);
			return (env_list[j]);
		}
		j++;
	}
	return (NULL);
}

char	*check_exp(t_cmd *cmd, t_minishell *shell, int var_len, int i)
{
	int		j;
	char	**exp_list;

	while (cmd->args[i][var_len] != '=' && cmd->args[i][var_len] != '\0')
		var_len++;
	exp_list = shell->exp_list;
	j = 0;
	while (exp_list[j] != NULL)
	{
		if (ft_strncmp(exp_list[j], cmd->args[i], var_len) == 0
			&& (exp_list[j][var_len] == '=' || exp_list[j][var_len] == '\0'))
		{
			shift_list(exp_list, j);
			return (exp_list[j]);
		}
		j++;
	}
	return (NULL);
}

int	builtin_unset(t_cmd *cmd, t_minishell *shell)
{
	int		i;
	int		var_len;

	i = 1;
	while (cmd->args[i] != NULL)
	{
		if (cmd->args[i][0] == '\0')
		{
			i++;
			continue ;
		}
		var_len = 0;
		while (cmd->args[i][var_len] != '=' && cmd->args[i][var_len] != '\0')
			var_len++;
		check_env(cmd, shell, var_len, i);
		check_exp(cmd, shell, var_len, i);
		i++;
	}
	return (SUCCESS);
}
