/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 16:55:01 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/21 11:13:16 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Configures the child's input/output file descriptors for a pipeline.
If prev fd is valid it becomes the child stdin */
static void	setup_child_io(t_cmd *current, int prev_fd, int *fd)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (current->next)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
}

/* Executes a single command in the child process */
static void	exec_command(t_minishell *shell, t_cmd *current)
{
	char	*path;
	int		status;

	status = run_builtin(current, shell);
	if (status != FAILURE)
	{
		shell->exit_code = status;
		exit(status);
	}
	path = find_cmd_path(current->args, shell->envp);
	if (!path)
	{
		print_error(shell, "command not found");
		shell->exit_code = 127;
		exit(127);
	}
	execve(path, current->args, shell->envp);
	print_error(shell, "execve failed");
	exit(126);
}

/* Entry point for child execution after fork() */
void	execute_child(t_minishell *shell, t_cmd *current, int prev_fd, int *fd)
{
	setup_child_signals();
	setup_child_io(current, prev_fd, fd);
	if (handle_redirects(current) == FAILURE)
	{
		shell->exit_code = 1;
		exit(1);
	}
	exec_command(shell, current);
}
