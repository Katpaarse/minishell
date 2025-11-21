/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:36:19 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/21 12:39:15 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Counts how many command nodes are in the pipeline by iterating 
through the linked list */
static int	count_commands(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

/* Creates the pipeline by forking one child process per command.
Passes the correct pipe file descriptors to each child.
Stores all child PIDs in the provided array.
Waits for all children after forking */
static int	fork_all_children(t_minishell *shell, t_cmd *cmd, pid_t *pids,
	int *fd)
{
	int		i;
	int		prev_fd;
	pid_t	pid;
	t_cmd	*current;

	current = cmd;
	prev_fd = -1;
	i = 0;
	while (current)
	{
		pid = fork_and_execute_child(shell, current, prev_fd, fd);
		if (pid == -1)
			return (FAILURE);
		pids[i] = pid;
		i++;
		if (current->next != NULL)
			prev_fd = fd[0];
		else
			prev_fd = -1;
		current = current->next;
	}
	wait_all_children(shell, pids, i);
	return (SUCCESS);
}

/* Runs an entire pipeline of commands */
void	execute_pipeline(t_minishell *shell, t_cmd *cmds)
{
	int		fd[2];
	int		num;
	pid_t	*pids;

	if (!cmds)
		return ;
	if (!cmds->next && is_parent_builtin(cmds))
	{
		shell->exit_code = run_builtin(cmds, shell);
		return ;
	}
	g_minishell_is_executing = 1;
	num = count_commands(cmds);
	pids = malloc(sizeof(pid_t) * num);
	if (!pids)
		return ;
	if (fork_all_children(shell, cmds, pids, fd) == FAILURE)
	{
		free(pids);
		g_minishell_is_executing = 0;
		return ;
	}
	free(pids);
	g_minishell_is_executing = 0;
}
