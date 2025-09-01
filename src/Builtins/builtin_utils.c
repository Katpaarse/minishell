/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 18:21:49 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/21 19:36:26 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(char **argv)
{
    if (argv[0])
    {
        if (ft_strncmp(argv[0], "cd", 3) == 0)
            return (SUCCESS);
		else if (ft_strncmp(argv[0], "echo", 5) == 0)
			return (SUCCESS);
        else if (ft_strncmp(argv[0], "pwd", 4) == 0)
			return (SUCCESS);
		else if (ft_strncmp(argv[0], "env", 4) == 0)
			return (SUCCESS);
        else if (ft_strncmp(argv[0], "export", 7) == 0)
            return (SUCCESS);
        else if (ft_strncmp(argv[0], "unset", 6) == 0)
            return (SUCCESS);
        else if (ft_strncmp(argv[0], "exit", 5) == 0)
            return (SUCCESS);
        else
            return (FAILURE);
    }
    else
        return (FAILURE);
}

int run_builtin(char **argv, t_minishell *shell, t_cmd *cmd)
{
    if (argv[0])
    {
        if (ft_strncmp(argv[0], "cd", 3) == 0)
            return (builtin_cd(cmd, shell));
		else if (ft_strncmp(argv[0], "echo", 5) == 0)
			return (builtin_echo(cmd));
        else if (ft_strncmp(argv[0], "pwd", 4) == 0)
			return (builtin_pwd());
		else if (ft_strncmp(argv[0], "env", 4) == 0)
			return (builtin_env(shell));
        else if (ft_strncmp(argv[0], "export", 7) == 0)
            return (builtin_export(cmd, shell));
        else if (ft_strncmp(argv[0], "unset", 6) == 0)
            return (builtin_unset(cmd, shell));
        else if (ft_strncmp(argv[0], "exit", 5) == 0)
            return (builtin_exit(cmd, shell));
        else
            return (FAILURE);
    }
    else
        return (FAILURE);
}

/*

This will automatically redirect stdin/stdout according to the command’s redirects array.

You don’t need to manually pass fds to each builtin.

Heredoc (<<) still needs implementation in is_redirect.

int	run_builtin(char **argv, t_minishell *shell, t_cmd *cmd)
{
	int		stdin_backup;
	int		stdout_backup;

	if (!argv || !cmd)
		return (FAILURE);

	// Backup original stdin and stdout
	stdin_backup = dup(0);
	stdout_backup = dup(1);

	// Apply redirections
	if (is_redirect(cmd) == FAILURE)
	{
		// If redirection fails, restore and exit
		dup2(stdin_backup, 0);
		dup2(stdout_backup, 1);
		close(stdin_backup);
		close(stdout_backup);
		return (FAILURE);
	}

	if (ft_strncmp(argv[0], "cd", 3) == 0)
		builtin_cd(cmd, shell);
	else if (ft_strncmp(argv[0], "echo", 5) == 0)
		builtin_echo(cmd);
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		builtin_pwd();
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		builtin_env(shell);
	else if (ft_strncmp(argv[0], "export", 7) == 0)
		builtin_export(cmd, shell);
	else if (ft_strncmp(argv[0], "unset", 6) == 0)
		builtin_unset(cmd, shell);
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		builtin_exit(cmd, shell);
	else
	{
		// Restore original stdin/stdout if builtin not recognized
		dup2(stdin_backup, 0);
		dup2(stdout_backup, 1);
		close(stdin_backup);
		close(stdout_backup);
		return (FAILURE);
	}

	// Restore original stdin/stdout after running builtin
	dup2(stdin_backup, 0);
	dup2(stdout_backup, 1);
	close(stdin_backup);
	close(stdout_backup);

	return (SUCCESS);
}
*/

int	is_redirect(t_cmd *cmd, t_minishell *shell)
{
	int fd;
	int	i;

	if (!cmd || !shell)
		return (FAILURE);

	i = 0;
	while (cmd->redirects[i].type != RED_NONE)
	{
		if (cmd->redirects[i].type == RED_OUTPUT)
			fd = open(cmd->redirects[i].filename, O_WRONLY | O_CREAT | O_TRUNC, 0666); 	// '>' TRUNCATE mode
		else if (cmd->redirects[i].type == RED_APPEND)
			fd = open(cmd->redirects[i].filename, O_WRONLY | O_CREAT | O_APPEND, 0666); // '>>' APPEND mode
		else if (cmd->redirects[i].type == RED_INPUT)
			fd = open(cmd->redirects[i].filename, O_RDONLY);							// '<' INFILE mode
		else if (cmd->redirects[i].type == RED_HEREDOC)
		{																				// '<<' HEREDOC 'EOF' mode
			// implement heredoc handling
			i++;
			continue ;
		}
		else
		{
			i++;
			continue ;
		}

		if (fd < 0)
		{
			// error handling here
			return (FAILURE);
		}

		// Redirect stdout or stdin depending on type
		if (cmd->redirects[i].type == RED_OUTPUT || cmd->redirects[i].type == RED_APPEND)
		{
			if (dup2(fd, 1) < 0)
			{
				// error handling here
				close(fd);
				return (FAILURE);
			}
		}
		else if (cmd->redirects[i].type == RED_INPUT || cmd->redirects[i].type == RED_HEREDOC)
		{
			if (dup2(fd, 0) < 0)
			{
				// error handling here
				close(fd);
				return (FAILURE);
			}
		}

		close(fd);
		i++;
	}

	return (SUCCESS);
}



