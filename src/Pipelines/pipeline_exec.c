/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:36:19 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/05 18:34:04 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirects(t_cmd *cmd)
{
	int			fd;
	t_redirect	*redirect;
	t_redirect	*last_heredoc;
	
	if (!cmd || !cmd->redir)
		return (SUCCESS);
	last_heredoc = NULL;
	redirect = cmd->redir;
	while (redirect) // First pass is to find the last redirect
	{
		if (redirect->type == RED_HEREDOC)
			last_heredoc = redirect;
		redirect = redirect->next;
	}
	// Second pass: process all redirects, but for heredocs only use the last one
	redirect = cmd->redir;
	while (redirect)
	{
		if (redirect->type == RED_HEREDOC || redirect->type == RED_INPUT)
		{
			// for heredocs, only use the last one
			if (redirect->type == RED_HEREDOC && redirect != last_heredoc)
			{
				redirect = redirect->next;
				continue ; //skip non last heredocs
			}
			fd = open(redirect->filename, O_RDONLY);
			if (fd < 0)
			{
				print_error_filename(redirect->filename, "No such file or directory");
				exit(1);
			}
			else
			{
				dup2(fd, 0);
				close(fd);
			}
		}
		else if (redirect->type == RED_OUTPUT)
		{
			fd = open(redirect->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				print_error_filename(redirect->filename, "Permission denied");
				exit(1);
			}
			else
			{
				dup2(fd, 1);
				close(fd);
			}
		}
		else if (redirect->type == RED_APPEND)
		{
			fd = open(redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
			{
				print_error_filename(redirect->filename, "Permission denied");
				exit(1);
			}
			else
			{
				dup2(fd, 1);
				close(fd);
			}
		}
		redirect = redirect->next;
	}
	return (SUCCESS);
}

//  Execute a child process (builtin or external)
void execute_child(t_minishell *shell, t_cmd *current, int prev_fd, int *fd)
{
	char		*cmd_path;

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
