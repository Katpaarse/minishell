/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:36:19 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/30 18:30:37 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirects(t_cmd *cmd)
{
	int			fd;
	t_redirect	*redirection;
	t_redirect	*last_heredoc;
	
	if (!cmd || !cmd->redirects)
	return (SUCCESS);
	last_heredoc = NULL;
	redirection = cmd->redirects;
	while (redirection) // First pass is to find the last redirect
	{
		if (redirection->type == RED_HEREDOC)
			last_heredoc = redirection;
		redirection = redirection->next;
	}
	// Second pass: process all redirects, but for heredocs only use the last one
	redirection = cmd->redirects;
	while (redirection)
	{
		if (redirection->type == RED_HEREDOC || redirection->type == RED_INPUT)
		{
			// for heredocs, only use the last one
			if (redirection->type == RED_HEREDOC && redirection != last_heredoc)
			{
				redirection = redirection->next;
				continue ; //skip non last heredocs
			}
			fd = open(redirection->filename, O_RDONLY);
			if (fd < 0)
				perror("open input/heredoc");
			else
			{
				dup2(fd, 0);
				close(fd);
			}
		}
		else if (redirection->type == RED_OUTPUT)
		{
			fd = open(redirection->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				perror("open output");
			else
			{
				dup2(fd, 1);
				close(fd);
			}
		}
		else if (redirection->type == RED_APPEND)
		{
			fd = open(redirection->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
				perror("open append");
			else
			{
				dup2(fd, 1);
				close(fd);
			}
		}
		redirection = redirection->next;
	}
	return (SUCCESS);
}

//  Execute a child process (builtin or external)
void execute_child(t_minishell *shell, t_cmd *current, int prev_fd, int *fd)
{
	char	*cmd_path;

	setup_child_signals(); 		// setup default signal handlers for child process
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
	handle_redirects(current);
	if (run_builtin(current, shell) != FAILURE) // Builtins like echo, pwd, etc. are executed directly. If a builtin was run, you exit the child process immediately with the builtin’s exit code.
		exit(shell->exit_code);
	cmd_path = find_cmd_path(current->args, shell->envp); // If it’s not a builtin, you look for an executable in $PATH
	if (!cmd_path)
	{
		print_error(shell, "command not found");
		exit(127);
	}
	execve(cmd_path, current->args, shell->envp); // If found -> replace the child process with the program.
	print_error(shell, "execve failed");
	exit(126);
}

void	execute_pipeline(t_minishell *shell, t_cmd *cmds)
{
	int		i;
	int		fd[2]; // initialize 2 unused fds. fd[0] = fd 3 (read end) and fd[1] = fd 4 (write end)
	int		prev_fd;
	t_cmd	*current;
	int		num_cmds;
	pid_t	*child_pids;
	pid_t	pid;

	if (!cmds)
		return ;
	// Special case: single parent-only builtin
	if (!cmds->next && is_parent_builtin(cmds)) // checks if there is not next command. So its not part of cmd1 | cmd2 | cmd3 | but something like echo hello | cd .. also checks if its a parent builtin so no child process is needed.
	{
		shell->exit_code = run_builtin(cmds, shell);
		return ;
	}
	g_minishell_is_executing = 1; // set global variable to indicate we're executing commands
	// Count number of commands
	num_cmds = 0;
	current = cmds;
	while (current)
	{
		num_cmds++;
		current = current->next;
	}
	// Allocate array for child PIDs
	child_pids = malloc(sizeof(pid_t) * num_cmds);
	if (!child_pids)
	{
		g_minishell_is_executing = 0; // reset on error		
		return ;
	}
	// Reset variables
	current = cmds;
	prev_fd = -1;
	i = 0;
	// Fork each command
	while (current)
	{
		pid = fork_and_execute_child(shell, current, prev_fd, fd);
		if (pid == -1)
		{
			free(child_pids);
			g_minishell_is_executing = 0; // reset on error
			return ;
		}
		child_pids[i] = pid;
		if (current->next != NULL)
			prev_fd = fd[0];
		else
			prev_fd = -1;
		current = current->next;
		i++;
	}
	// Wait for all children
	wait_all_children(shell, child_pids, num_cmds);
	free(child_pids);
	g_minishell_is_executing = 0; // reset variable after execution completes
}

/*
We’ll use the example command:
echo hello | grep h | wc -c
Your minishell builds a linked list:
cmd1: echo hello
cmd2: grep h
cmd3: wc -c
Step 0: Initial FDs

At the very beginning, each process has at least:
0 → stdin (terminal input)
1 → stdout (terminal output)
2 → stderr (terminal error)
Everything else is unused (FD 3+).

Step 1: First command (echo hello)
Parent creates pipe1 → returns 2 new FDs:
fd[0] = 3 (read end)
fd[1] = 4 (write end)
Child1 (echo)
dup2(fd[1], 1) → duplicate 4 onto 1 (stdout now points to pipe write).
Close original pipe FDs (3 and 4).
Now child1 has:
stdin = 0 (terminal)
stdout = 1 (really pipe write end)
Runs echo hello, writes into pipe1.
Parent
Closes old prev_fd (none yet).
Closes fd[1] = 4 (write end of pipe1).
Keeps fd[0] = 3 for next child.
Step 2: Second command (grep h)
Parent creates pipe2 → returns new FDs:
fd[0] = 5 (read end)
fd[1] = 6 (write end)
Child2 (grep)
dup2(prev_fd, 0) → dup2(3, 0) → stdin now comes from pipe1 read end.
dup2(fd[1], 1) → dup2(6, 1) → stdout now points to pipe2 write end.
Close FDs 3, 5, 6.
Now child2 has:
stdin = 0 (pipe1 read end)
stdout = 1 (pipe2 write end)
Runs grep h, reads from echo output, writes into pipe2.
Parent
Closes prev_fd = 3 (pipe1 read end).
Closes fd[1] = 6 (pipe2 write end).
Keeps fd[0] = 5 for next child.
Step 3: Third command (wc -c)
current->next == NULL → no new pipe.
Child3 (wc)
dup2(prev_fd, 0) → dup2(5, 0) → stdin now comes from pipe2 read end.
stdout remains = 1 (terminal).
Close FD 5.
Now child3 has:
stdin = 0 (pipe2 read end)
stdout = 1 (terminal)
Runs wc -c, reads from grep output, writes to terminal.
Parent
Closes prev_fd = 5.

Step 4: Parent waits

Parent calls waitpid on all 3 children.
shell->exit_code is updated to the exit code of the last child (wc -c).

Child1	terminal	pipe1 (fd=4 → dup’d to 1)	writes hello\n
Child2	pipe1 (fd=3 → dup’d to 0)	pipe2 (fd=6 → dup’d to 1)	filters text
Child3	pipe2 (fd=5 → dup’d to 0)	terminal	counts characters
Parent	closes unused ends, only waits	terminal	coordinates

After execution:
Parent has closed all pipe ends (3, 4, 5, 6).

Only 0, 1, 2 remain.

That’s how your pipeline wiring works under the hood.
Every pipe adds two new FDs. You duplicate (dup2) the one you need (read for stdin, write for stdout), then close both originals so only the correct processes keep the pipe ends they need.
*/