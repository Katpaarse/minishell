/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:53:00 by lavan-de          #+#    #+#             */
/*   Updated: 2025/11/05 16:44:35 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	child_redirects(t_cmd *cmd, t_minishell *shell, int result)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (FAILURE);
	if (pid == 0)
	{
		setup_child_signals();
		if (handle_redirects(cmd) == FAILURE)
			_exit(EXIT_FAILURE);
		result = execute_builtin(cmd, shell);
		_exit(result);
	}
	waitpid(pid, &status, 0);
	return (status / 256);
}

int	check_redirects(t_cmd *cmd, int s_stdin, int s_stdout)
{
	if (handle_redirects(cmd) == FAILURE)
	{
		if (s_stdout >= 0)
			close(s_stdout);
		if (s_stdin >= 0)
			close(s_stdin);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	run_builtin(t_cmd *cmd, t_minishell *shell)
{
	int		saved_stdout;
	int		saved_stdin;
	int		result;

	saved_stdout = -1;
	saved_stdin = -1;
	result = 0;
	if (!cmd->args[0])
		return (FAILURE);
	if (is_parent_builtin(cmd) == SUCCESS)
	{
		if (cmd->redir)
		{
			saved_stdout = dup(STDOUT_FILENO);
			saved_stdin = dup(STDIN_FILENO);
			if (saved_stdout == -1 || saved_stdin == -1)
			{
				if (saved_stdout != -1)
					close(saved_stdout);
				if (saved_stdin != -1)
					close(saved_stdin);
				return (FAILURE);
			}
		}
		if (check_redirects(cmd, saved_stdin, saved_stdout) == FAILURE)
			return (FAILURE);
		result = execute_builtin(cmd, shell);
		shell->exit_code = result;
		if (cmd->redir)
		{
			if (saved_stdout != -1)
			{
				if (dup2(saved_stdout, STDOUT_FILENO) == -1)
					result = FAILURE;
				close(saved_stdout);
			}
			if (saved_stdin != -1)
			{
				if (dup2(saved_stdin, STDIN_FILENO) == -1)
					result = FAILURE;
				close(saved_stdin);
			}
		}
		return (result);
	}
	else
	{
		if (cmd->redir)
		{
			result = child_redirects(cmd, shell, result);
			shell->exit_code = result;
			return (result);
		}
		else
		{
			result = execute_builtin(cmd, shell);
			shell->exit_code = result;
			return (result);
		}
	}
}

int	run_external(t_cmd *cmd, t_minishell *shell)
{
	char	*cmd_path;

	if (ft_strchr(cmd->args[0], '/'))
		return (execute_path_cmd(cmd, shell));
	cmd_path = find_cmd_path(cmd->args, shell->envp);
	if (!cmd_path)
	{
		print_error(shell, "command not found");
		return (127);
	}
	if (access(cmd_path, X_OK) != 0)
	{
		print_error(shell, "Permission denied");
		free(cmd_path);
		return (126);
	}
	cmd_fork(cmd, shell, cmd_path);
	free(cmd_path);
	return (shell->exit_code);
}

int	cmd_fork(t_cmd *cmd, t_minishell *shell, char *ext_cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(ext_cmd);
		return (FAILURE);
	}
	if (pid == 0)
	{
		setup_child_signals();
		if (handle_redirects(cmd) == FAILURE)
			_exit(1);
		execve(ext_cmd, cmd->args, shell->envp);
		perror("execve failed");
		_exit(126);
	}
	g_minishell_is_executing = pid;
	shell->exit_code = wait_for_child(pid);
	g_minishell_is_executing = 0;
	return (shell->exit_code);
}
