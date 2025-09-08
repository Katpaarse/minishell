/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:36:19 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/08 19:07:07 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	execute_pipeline(t_minishell *shell, t_cmd cmds)
{
	
}
