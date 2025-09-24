/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:36:06 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/24 17:57:15 by jukerste         ###   ########.fr       */
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
		execute_child(shell, current, prev_fd, fd); // In the child -> run execute_child() to set up FDs and run the command.
	if (pid > 0)
	{
		if (prev_fd != -1) // -1 is sentinal. So there was no previous fd used.
			close(prev_fd);
		if (current->next != NULL)
			close(fd[1]);
	}
	return (pid);
}

// Wait for all children (while loop). Use wait_for_child function from builtin utils
void wait_all_children(t_minishell *shell, pid_t *child_pids, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		shell->exit_code = wait_for_child(child_pids[i]);
		i++;
	}
}
