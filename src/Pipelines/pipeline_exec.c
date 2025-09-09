/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:36:19 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/09 14:46:35 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Pipelines/ folder
→ handles all the gritty stuff:

	looping over commands,

	creating pipes,

	forking children,

	redirecting stdin/stdout with dup2,

	running builtins or externals in the child,

	closing fds properly,

	waiting and collecting exit statuses.
*/

void	execute_pipeline(t_minishell *shell, t_cmd cmds)
{
	
}

//  Execute a child process (builtin or external)
static void execute_child(t_minishell *shell, t_cmd *cur, int prev_fd, int *fd)
{
	
}

// Fork and execute child, setup pipes
static pid_t fork_and_execute_child(t_minishell *shell, t_cmd *cur, int prev_fd, int *fd)
{
	
}

// Wait for all children (while loop). Use wait_for_child function from builtin utils
static void wait_all_children(t_minishell *shell, pid_t *child_pids, int count)
{
	
}

