/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:36:06 by lavan-de          #+#    #+#             */
/*   Updated: 2025/10/03 18:40:25 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Fork and execute child, setup pipes
pid_t fork_and_execute_child(t_minishell *shell, t_cmd *current, int prev_fd, int *fd)
{
	pid_t	pid;

	if (current->next != NULL && pipe(fd) == -1) // If not the last command -> create a new pipe for communication with the next child
	{
		print_error(shell, "pipe failed");
		return (-1);
	}
	pid = fork(); // clone process for child process
	if (pid < 0)
	{
		print_error(shell, "fork failed");
		if (current->next != NULL)
		{
			close(fd[0]);
			close(fd[1]);
		}
		return (-1);
	}
	if (pid == 0) // if pid is 0. Its a child process. Parent process has a original procces id
	{
		setup_child_signals();
		execute_child(shell, current, prev_fd, fd); // In the child -> run execute_child() to set up FDs and run the command.
		exit(EXIT_FAILURE);
	}
	if (pid > 0)
	{
		if (prev_fd != -1) // -1 is sentinal. So there was no previous fd used.
			close(prev_fd);
		if (current->next != NULL)
			close(fd[1]);
	}
	return (pid);
}

// used for pipelines, wait for all, track last status
int wait_all_children(t_minishell *shell, pid_t *child_pids, int count)
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
		// shell->exit_code = wait_for_child(child_pids[i]);
		i++;
	}

	// if (WIFSIGNALED(status))
	// {
		// if (WTERMSIG(status) == SIGINT)
			// write(1, "\n", 1);
		if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
	// }

	shell->exit_code = exit_code;
	// printf("%d\n", shell->exit_code);
	return (exit_code);
}
