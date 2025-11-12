/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 18:21:49 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/30 18:30:03 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Must run in parent: cd, export, unset, exit
int	is_parent_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (FAILURE);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (SUCCESS);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (SUCCESS);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (SUCCESS);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (SUCCESS);
	else
		return (FAILURE);
}

int	is_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (FAILURE);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (SUCCESS);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (SUCCESS);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (SUCCESS);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (SUCCESS);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (SUCCESS);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (SUCCESS);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (SUCCESS);
	else
		return (FAILURE);
}

int	execute_builtin(t_cmd *cmd, t_minishell *shell)
{
	if (cmd->args[0])
	{
		if (ft_strcmp(cmd->args[0], "cd") == 0)
			return (builtin_cd(cmd, shell));
		else if (ft_strcmp(cmd->args[0], "echo") == 0)
			return (builtin_echo(cmd));
		else if (ft_strcmp(cmd->args[0], "pwd") == 0)
			return (builtin_pwd());
		else if (ft_strcmp(cmd->args[0], "env") == 0)
			return (builtin_env(shell));
		else if (ft_strcmp(cmd->args[0], "export") == 0)
			return (builtin_export(cmd, shell));
		else if (ft_strcmp(cmd->args[0], "unset") == 0)
			return (builtin_unset(cmd, shell));
		else if (ft_strcmp(cmd->args[0], "exit") == 0)
			return (builtin_exit(cmd, shell));
		else
			return (FAILURE);
	}
	else
		return (FAILURE);
}

void	write_export(t_minishell *shell)
{
	int		j;
	char	*equal_sign;

	j = 0;
	while (shell->exp_list && shell->exp_list[j] != NULL)
	{
		write(1, "declare -x ", 11);
		if (ft_strchr(shell->exp_list[j], '='))
		{
			equal_sign = ft_strchr(shell->exp_list[j], '=');
			write(1, shell->exp_list[j], equal_sign - shell->exp_list[j] + 1);
			write(1, "\"", 1);
			write(1, equal_sign + 1, ft_strlen(equal_sign + 1));
			write(1, "\"", 1);
		}
		else
			write(1, shell->exp_list[j], ft_strlen(shell->exp_list[j]));
		write(1, "\n", 1);
		j++;
	}
	return ;
}
