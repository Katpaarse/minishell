/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:53:00 by lavan-de          #+#    #+#             */
/*   Updated: 2025/10/30 17:53:02 by lavan-de         ###   ########.fr       */
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
			exit(EXIT_FAILURE);
		result = execute_builtin(cmd, shell);
		exit(result);
	}
	waitpid(pid, &status, 0);
	return (status / 256);
}

int	check_redirects(t_cmd *cmd, int s_stdin, int s_stdout)
{
	if (cmd->redirects)
	{
		s_stdout = dup2(s_stdout, STDOUT_FILENO);
		s_stdin = dup2(s_stdin, STDIN_FILENO);
		if (s_stdout < 0 || s_stdin < 0)
		{
			if (s_stdout >= 0)
				close(s_stdout);
			if (s_stdin >= 0)
				close(s_stdin);
			return (FAILURE);
		}
	}
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
		check_redirects(cmd, saved_stdin, saved_stdout);
		result = execute_builtin(cmd, shell);
		return (result);
	}
	else
	{
		if (cmd->redirects)
			return (child_redirects(cmd, shell, result));
		else
			return (execute_builtin(cmd, shell));
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
		handle_redirects(cmd);
		execve(ext_cmd, cmd->args, shell->envp);
		perror("execve failed");
		exit(126);
	}
	g_minishell_is_executing = pid;
	shell->exit_code = wait_for_child(pid);
	g_minishell_is_executing = 0;
	return (shell->exit_code);
}
