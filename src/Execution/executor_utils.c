/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:53:00 by lavan-de          #+#    #+#             */
/*   Updated: 2025/11/18 13:29:16 by jukerste         ###   ########.fr       */
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

static int	setup_parent_redirects(t_cmd *cmd, int *saved_in, int *saved_out)
{
	*saved_in = -1;
	*saved_out = -1;
	if (!cmd->redir)
		return (SUCCESS);
	*saved_out = dup(STDOUT_FILENO);
	*saved_in = dup(STDIN_FILENO);
	if (*saved_out == -1 || *saved_in == -1)
	{
		if (*saved_out != -1) 
			close(*saved_out);
		if (*saved_in != -1) 
			close(*saved_in);
		*saved_out = -1;
		*saved_in = -1;
		return (FAILURE);
	}
	if (check_redirects(cmd, *saved_in, *saved_out) == FAILURE)
	{
		close(*saved_out);
		close(*saved_in);
		*saved_out = -1;
		*saved_in = -1;
		return (FAILURE);
	}
	return (SUCCESS);
}

static int	restore_parent_redirects(int saved_in, int saved_out, int result)
{
	int	status;

	status = result;
	if (saved_out != -1 && dup2(saved_out, STDOUT_FILENO) == -1)
		status = FAILURE;
	if (saved_in != -1 && dup2(saved_in, STDIN_FILENO) == -1)
		status = FAILURE;
	if (saved_out != -1) 
		result = close(saved_out);
	if (saved_in != -1) 
		result = close(saved_in);
	return (status);
}

static int	run_parent_builtin(t_cmd *cmd, t_minishell *shell)
{
	int	saved_in;
	int	saved_out;
	int	result;

	if (setup_parent_redirects(cmd, &saved_in, &saved_out) == FAILURE)
		return (FAILURE);
	result = execute_builtin(cmd, shell);
	shell->exit_code = result;
	return (restore_parent_redirects(saved_in, saved_out, result));
}

static int	run_child_builtin(t_cmd *cmd, t_minishell *shell)
{
	int	result;

	if (cmd->redir)
		result = child_redirects(cmd, shell, 0);
	else
		result = execute_builtin(cmd, shell);
	shell->exit_code = result;
	return (result);
}

int	run_builtin(t_cmd *cmd, t_minishell *shell)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (FAILURE);
	if (is_parent_builtin(cmd) == SUCCESS)
		return (run_parent_builtin(cmd, shell));
	return (run_child_builtin(cmd, shell));
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
			exit(1);
		execve(ext_cmd, cmd->args, shell->envp);
		perror("execve failed");
		exit(126);
	}
	g_minishell_is_executing = pid;
	shell->exit_code = wait_for_child(pid);
	g_minishell_is_executing = 0;
	return (shell->exit_code);
}
