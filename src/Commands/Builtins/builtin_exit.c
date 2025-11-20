/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:09 by lavan-de          #+#    #+#             */
/*   Updated: 2025/11/20 13:35:54 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_exit(t_cmd *cmd, t_minishell *shell)
{
	int		i;

	i = 0;
	if (cmd->args[1][0] == '-' || cmd->args[1][0] == '+')
		i++;
	if (cmd->args[1][i] == '\0')
	{
		write(2, "minishell: exit: ", 17);
		write(2, cmd->args[1], ft_strlen(cmd->args[1]));
		write(2, ": numeric argument required\n", 28);
		free_minishell(shell);
		exit(2);
	}
	while (cmd->args[1][i] != '\0')
	{
		if (!ft_isdigit(cmd->args[1][i]))
		{
			write(2, "minishell: exit: ", 17);
			write(2, cmd->args[1], ft_strlen(cmd->args[1]));
			write(2, ": numeric argument required\n", 28);
			free_minishell(shell);
			exit(2);
		}
		i++;
	}
}

static int	is_within_long_range(char *str)
{
	unsigned long long	value;
	unsigned long long	limit;
	int					i;

	if (str[0] == '-' || str[0] == '+')
		i = 1;
	else
		i = 0;
	if (str[0] == '-')
		limit = (unsigned long long)LONG_MAX + 1;
	else
		limit = (unsigned long long)LONG_MAX;
	value = 0;
	while (str[i])
	{
		value = value * 10 + (str[i] - '0');
		if (value > limit)
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	builtin_exit(t_cmd *cmd, t_minishell *shell)
{
	long	exit_code;

	write(1, "exit\n", 5);
	if (cmd->args[1] && cmd->args[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (FAILURE);
	}
	if (!cmd->args[1])
	{
		free_minishell(shell);
		exit(shell->exit_code);
	}
	check_exit(cmd, shell);
	if (!is_within_long_range(cmd->args[1]))
	{
		write(2, "minishell: exit: ", 17);
		write(2, cmd->args[1], ft_strlen(cmd->args[1]));
		write(2, ": numeric argument required\n", 28);
		free_minishell(shell);
		exit(2);
	}
	exit_code = ft_atol(cmd->args[1]);
	free_minishell(shell);
	exit(exit_code % 256);
}
