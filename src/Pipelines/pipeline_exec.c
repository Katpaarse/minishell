/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:36:19 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/09 19:53:28 by jukerste         ###   ########.fr       */
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

//  Execute a child process (builtin or external)
static void execute_child(t_minishell *shell, t_cmd *current, int prev_fd, int *fd)
{
	char	*cmd_path;
	
	if (prev_fd != -1)
	{
		dup2(prev_fd, 0); // new fd and old fd. Closes the old fd which is stdin in this case (0)
		close(prev_fd);
	}
	if (current->next != NULL)
	{
		dup2(fd[1], 1); // replace stdout with new pipe's write end
		close(fd[0]); // close read end
		close(fd[1]); // close write end after dup
	}
	if (run_builtin(current, shell) != FAILURE) // Builtins like echo, pwd, etc. are executed directly. If a builtin was run, you exit the child process immediately with the builtin’s exit code.
		exit(shell->exit_code);
	cmd_path = find_cmd_path(current->args, shell->envp); // If it’s not a builtin, you look for an executable in $PATH
	if (cmd_path == NULL)
	{
		print_error(shell, "command not found");
		exit(127);
	}
	execve(cmd_path, current->args, shell->envp); // If found -> replace the child process with the program.
	print_error(shell, "execve failed");
	exit(126);
}

// Fork and execute child, setup pipes
static pid_t fork_and_execute_child(t_minishell *shell, t_cmd *current, int prev_fd, int *fd)
{
	pid_t	pid;

	if (current->next != NULL) // If not the last command -> create a new pipe for communication with the next child
	{
		if(pipe(fd) == -1)
		{
			print_error(shell, "pipe failed");
			return (-1);
		}
	}
	pid = fork(); // clone process for child process
	if (pid < 0)
	{
		print_error(shell, "fork failed");
		return (-1);
	}
	if (pid == 0)
		execute_child(shell, current, prev_fd, fd); // In the child -> run execute_child() to set up FDs and run the command.
	if (prev_fd != -1)
		close(prev_fd);
	if (current->next != NULL)
		prev_fd = fd[0];
	return (pid);
}

// Wait for all children (while loop). Use wait_for_child function from builtin utils
static void wait_all_children(t_minishell *shell, pid_t *child_pids, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		shell->exit_code = wait_for_child(child_pids[i]);
		i++;
	}
}
void	execute_pipeline(t_minishell *shell, t_cmd cmds)
{
	
}