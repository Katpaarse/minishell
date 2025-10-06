/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 16:24:09 by lavan-de          #+#    #+#             */
/*   Updated: 2025/10/03 19:45:26 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_external(t_cmd *cmd, t_minishell *shell)
{
	pid_t	pid;
	char 	*cmd_path;

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		print_error(shell, "invalid command");
		return (127);
	}
	cmd_path = find_cmd_path(cmd->args, shell->envp);
	if (!cmd_path)
	{
		print_error(shell, "command not found");
		return (127);
	}
	if (access(cmd_path, X_OK) != 0)
	{
		print_error(shell, "permission denied");
		free(cmd_path);
		return (126);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(cmd_path);
		return (FAILURE);
	}
	if (pid == 0)
	{
		setup_child_signals(); 		// setup default signal handlers for child process
		handle_redirects(cmd);
		execve(cmd_path, cmd->args, shell->envp); 	// replaces child process with new program
		perror("execve failed"); 		// only reached if execve fails
		exit(EXIT_FAILURE); 			// exit child process if execve fails
	}
	free(cmd_path);
	shell->exit_code = wait_for_child(pid);
	return (shell->exit_code);
}
