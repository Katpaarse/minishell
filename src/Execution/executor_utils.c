/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:53:00 by lavan-de          #+#    #+#             */
/*   Updated: 2025/11/20 17:04:56 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Forks and executes a builtin in a child process with redirects. When builtins
need redirection, isolates file descriptor changes to a child process */
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

/* Wrapper to handle redirects and clean up FDs on failure
Attempts to set up redirects and closes any opened FDs if an error occurs */
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

/* Saves stdin/stdout and applies redirects for parent builtins. Duplicates
current std FDs so they can be restored after the builtin finishes */
int	setup_parent_redirects(t_cmd *cmd, int *saved_in, int *saved_out)
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

/* Restores original stdin/stdout after parent builtin execution. Resets
 FDs to their saved state using dup2 and closes the backups */
int	restore_parent_redirects(int saved_in, int saved_out, int result)
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

/* Forks and executes an external command. Creates a child process, 
handles signals/redirects, and calls execve to run the program */
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
