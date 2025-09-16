/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 16:24:09 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/31 16:24:10 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_external(t_cmd *cmd, t_minishell *shell)
{
	pid_t	pid;
	int		status;
	char 	*cmd_path;

	cmd_path = find_cmd_path(cmd->args, shell->envp);
	if (!cmd_path)
	{
		// error handling can be added here if needed
		return (127);
	}
	
	pid = fork();
	if (pid < 0)
	{
		// error handling can be added here if needed
		return (1);
	}

	if (pid == 0)
	{
		if (is_redirect(cmd, shell) == FAILURE)
			exit(EXIT_FAILURE); 			// exit child process if redirection fails
		execve(cmd_path, cmd->args, shell->envp); 	// replaces child process with new program
		perror("execve failed"); 		// only reached if execve fails
		exit(EXIT_FAILURE); 			// exit child process if execve fails
	}

	status = wait_for_child(pid);

	return (status);
}
