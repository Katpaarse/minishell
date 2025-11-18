/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:09 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/25 19:40:51 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	check_exit(t_cmd *cmd, t_minishell *shell)
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

int	is_within_long_range(char *str)
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



/* ----- COMMANDS ----- */
/* Builtins */
int		builtin_cd(t_cmd *cmd, t_minishell *shell);
int		new_old_pwd(t_cmd *cmd, t_minishell *shell);
char	*change_dir(t_cmd *cmd, t_minishell *shell, char *old_pwd);
void	set_pwd_env(t_cmd *cmd, t_minishell *shell, char *n_p, char *o_p);

int		builtin_echo(t_cmd *cmd);
int		skip_nl(t_cmd *cmd, int i);
void	write_echo(t_cmd *cmd, int i);

int		builtin_env(t_minishell *shell);

int		builtin_exit(t_cmd *cmd, t_minishell *shell);
void	check_exit(t_cmd *cmd, t_minishell *shell);


int		builtin_export(t_cmd *cmd, t_minishell *shell);
int		builtin_pwd(void);
int		builtin_unset(t_cmd *cmd, t_minishell *shell);


/* Externals */



/* ----- EXECUTION ----- */

/* ----- INPUTOUTPUT ----- */

/* ----- PARSING ----- */

/* ----- UTILS ----- */
