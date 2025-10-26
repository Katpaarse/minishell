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

// Must run in parent: cd, export, unset, exit -> they change shell state. Can run in child process: echo, pwd, env -> they output stuff
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

int	run_builtin(t_cmd *cmd, t_minishell *shell)
{
	int		saved_stdout;
	int		saved_stdin;
	int		result;
	int		status;
	pid_t	pid;

	saved_stdout = -1;
	saved_stdin = -1;
	if (!cmd->args[0])
		return (FAILURE);
	if (is_parent_builtin(cmd) == SUCCESS)
	{
		if (cmd->redirects)
		{
			saved_stdout = dup(STDOUT_FILENO);
			saved_stdin = dup(STDIN_FILENO);
			if (saved_stdout < 0 || saved_stdin < 0)
			{
				if (saved_stdout >= 0)
					close(saved_stdout);
				if (saved_stdin >= 0)
					close(saved_stdin);
				return (FAILURE);
			}
		}
		if (handle_redirects(cmd) == FAILURE)
		{
			if (saved_stdout >= 0)
				close(saved_stdout);
			if (saved_stdin >= 0)
				close(saved_stdin);
			return (FAILURE);
		}
		result = execute_builtin(cmd, shell);
		if (cmd->redirects)
		{
			if (saved_stdout >= 0)
			{
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdout);
			}
			if (saved_stdin >= 0)
			{
				dup2(saved_stdin, STDIN_FILENO);
				close(saved_stdin);
			}
		}
		return (result);
	}
	else
	{
		if (cmd->redirects)
		{
			pid = fork();
			if (pid < 0)
				return (1);
			if (pid == 0)
			{
				setup_child_signals();
				if (handle_redirects(cmd) == FAILURE)
					exit(EXIT_FAILURE);
				result = execute_builtin(cmd, shell);
				exit(result);
			}
			waitpid(pid, &status, 0);
			return (status / 256);
		}
		else
			return (execute_builtin(cmd, shell));
	}
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
