/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:36:06 by lavan-de          #+#    #+#             */
/*   Updated: 2025/11/21 12:59:55 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_pipe_if_needed(t_minishell *shell, t_cmd *current, int *fd)
{
	if (current->next != NULL && pipe(fd) == -1)
	{
		print_error(shell, "pipe failed");
		return (-1);
	}
	return (0);
}

/* Prints an error message and closes any pipe fd's if they were created */
static pid_t	handle_fork_error(t_minishell *shell, t_cmd *current, int *fd)
{
	print_error(shell, "fork failed");
	if (current->next != NULL)
	{
		close(fd[0]);
		close(fd[1]);
	}
	return (-1);
}

pid_t	fork_and_execute_child(t_minishell *shell, t_cmd *current,
	int prev_fd, int *fd)
{
	pid_t	pid;

	if (create_pipe_if_needed(shell, current, fd) == -1)
		return (-1);
	pid = fork();
	if (pid < 0)
		return (handle_fork_error(shell, current, fd));
	if (pid == 0)
	{
		setup_child_signals();
		execute_child(shell, current, prev_fd, fd);
		exit(EXIT_FAILURE);
	}
	if (prev_fd != -1)
		close(prev_fd);
	if (current->next != NULL)
		close(fd[1]);
	return (pid);
}

/* Waits for all child processes in the pipeline to finish */
int	wait_all_children(t_minishell *shell, pid_t *child_pids, int count)
{
	int	i;
	int	status;
	int	exit_code;

	exit_code = 0;
	i = 0;
	while (i < count)
	{
		waitpid(child_pids[i], &status, 0);
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_code = 128 + WTERMSIG(status);
		i++;
	}
	if (WTERMSIG(status) == SIGQUIT)
		write(1, "Quit (core dumped)\n", 19);
	shell->exit_code = exit_code;
	return (exit_code);
}
