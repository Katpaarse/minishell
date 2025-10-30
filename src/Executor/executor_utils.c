/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:53:00 by lavan-de          #+#    #+#             */
/*   Updated: 2025/10/30 17:53:02 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_builtin(t_cmd *cmd, t_minishell *shell)
{
	int		saved_stdout;
	int		saved_stdin;
	int		result;
	int		status;
	pid_t	pid;

	saved_stdout = -1;
	saved_stdin = -1;
	if (!cmd->args[0])
		return (FAILURE);
	if (is_parent_builtin(cmd) == SUCCESS)
	{
		if (cmd->redirects)
		{
			saved_stdout = dup(STDOUT_FILENO);
			saved_stdin = dup(STDIN_FILENO);
			if (saved_stdout < 0 || saved_stdin < 0)
			{
				if (saved_stdout >= 0)
					close(saved_stdout);
				if (saved_stdin >= 0)
					close(saved_stdin);
				return (FAILURE);
			}
		}
		if (handle_redirects(cmd) == FAILURE)
		{
			if (saved_stdout >= 0)
				close(saved_stdout);
			if (saved_stdin >= 0)
				close(saved_stdin);
			return (FAILURE);
		}
		result = execute_builtin(cmd, shell);
		if (cmd->redirects)
		{
			if (saved_stdout >= 0)
			{
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdout);
			}
			if (saved_stdin >= 0)
			{
				dup2(saved_stdin, STDIN_FILENO);
				close(saved_stdin);
			}
		}
		return (result);
	}
	else
	{
		if (cmd->redirects)
		{
			pid = fork();
			if (pid < 0)
				return (1);
			if (pid == 0)
			{
				setup_child_signals();
				if (handle_redirects(cmd) == FAILURE)
					exit(EXIT_FAILURE);
				result = execute_builtin(cmd, shell);
				exit(result);
			}
			waitpid(pid, &status, 0);
			return (status / 256);
		}
		else
			return (execute_builtin(cmd, shell));
	}
}

int run_external(t_cmd *cmd, t_minishell *shell)
{
    // pid_t	pid;
    char 	*cmd_path;

    // if (!cmd || !cmd->args || !cmd->args[0])
    // {
    //     print_error(shell, "Invalid command");
    //     return (127);
    // }

    if (ft_strchr(cmd->args[0], '/'))
        return execute_path_cmd(cmd, shell);

    cmd_path = find_cmd_path(cmd->args, shell->envp);
    if (!cmd_path)
    {
        print_error(shell, "command not found");
        return (127);
    }
    if (access(cmd_path, X_OK) != 0)
    {
        print_error(shell, "Permission denied");
        free(cmd_path);
        return (126);
    }

    // pid = fork();
    // if (pid < 0)
    // {
    //     perror("fork");
    //     free(cmd_path);
    //     return (FAILURE);
    // }
    // if (pid == 0)
    // {
    //     setup_child_signals();
    //     handle_redirects(cmd);
    //     execve(cmd_path, cmd->args, shell->envp);
    //     perror("execve failed");
    //     exit(126);
    // }
    // free(cmd_path);
    // g_minishell_is_executing = pid;
    // shell->exit_code = wait_for_child(pid);
    // g_minishell_is_executing = 0;
	cmd_fork(cmd, shell, cmd_path);
	free(cmd_path);
    return (shell->exit_code);
}

int cmd_fork(t_cmd *cmd, t_minishell *shell, char *ext_cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(ext_cmd);
		return (FAILURE);
	}
	if (pid == 0)
	{
		setup_child_signals();
		handle_redirects(cmd);
		execve(ext_cmd, cmd->args, shell->envp);
		perror("execve failed");
		exit(126);
	}
	g_minishell_is_executing = pid;
	shell->exit_code = wait_for_child(pid);
	g_minishell_is_executing = 0;
	return (shell->exit_code);
}