/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:11 by lavan-de          #+#    #+#             */
/*   Updated: 2025/10/22 15:44:55 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_input(t_cmd *cmd, int i)
{
	int	j;
	int	check;

	j = 0;
	check = -1;
	if ((cmd->args[i][0] == '_' || ft_isalpha(cmd->args[i][0])) == 1)
	{
		while (cmd->args[i][j] && cmd->args[i][j] != '=')
		{
			if (cmd->args[i][j] != '_' && ft_isalpha(cmd->args[i][j]) == 0
				&& ft_isdigit(cmd->args[i][j]) == 0)
				return (check);
			j++;
		}
		if (ft_strchr(cmd->args[i], '=') != NULL)
		{
			if (cmd->args[i][j + 1] != '\0')
				check = 0;
			else
				check = 1;
		}
		else
			check = 2;
	}
	return (check);
}

int	check_list(char **list, char *var)
{
	int		i;
	int		len;
	int		size;

	len = 0;
	while (var[len] && var[len] != '=')
		len++;
	i = 0;
	while (list && list[i])
	{
		if (ft_strncmp(list[i], var, len) == 0
			&& (list[i][len] == '=' || list[i][len] == '\0'))
		{
			free(list[i]);
			list[i] = ft_strdup(var);
		}
		i++;
	}
	size = 0;
	while (list && list[size])
		size++;
	return (size);
}

void	add_or_update_env(t_minishell *shell, char *var)
{
	int		i;
	int		size;
	char	**new_env;

	if (!var)
		return ;
	size = check_list(shell->envp, var);
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return ;
	i = 0;
	while (i < size)
	{
		new_env[i] = shell->envp[i];
		i++;
	}
	new_env[size] = ft_strdup(var);
	new_env[size + 1] = NULL;
	free(shell->envp);
	shell->envp = new_env;
}

void	add_or_update_exp(t_minishell *shell, char *var)
{
	int		i;
	int		size;
	char	**new_exp;

	if (!var)
		return ;
	size = check_list(shell->exp_list, var);
	new_exp = malloc(sizeof(char *) * (size + 2));
	if (!new_exp)
		return ;
	i = 0;
	while (i < size)
	{
		new_exp[i] = shell->exp_list[i];
		i++;
	}
	new_exp[size] = ft_strdup(var);
	new_exp[size + 1] = NULL;
	free(shell->exp_list);
	shell->exp_list = new_exp;
}

int	builtin_export(t_cmd *cmd, t_minishell *shell)
{
	int	i;
	int	check;

	if (cmd->args[1] == NULL)
		return (write_export(shell), SUCCESS);
	i = 1;
	while (cmd->args[i])
	{
		check = check_input(cmd, i);
		if (check == 0 || check == 1)
		{
			add_or_update_exp(shell, cmd->args[i]);
			add_or_update_env(shell, cmd->args[i]);
		}
		else if (check == 2)
			add_or_update_exp(shell, cmd->args[i]);
		else
		{
			print_error(shell, "not a valid identifier\n");
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}
