/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 16:37:59 by lavan-de          #+#    #+#             */
/*   Updated: 2025/10/03 19:45:54 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_cmd_path(char **argv, char **envp)
{
	char	*path;

	if (!argv || !argv[0] || argv[0][0] == '\0' || !envp)
		return (NULL);

	if (ft_strchr(argv[0], '/') != NULL)
	{
		path = find_absolute_path(argv);
		return (path);
	}
	else
	{
		path = find_relative_path(argv[0], envp);
		return (path);
	}
	return (NULL);
}

char	*find_absolute_path(char **argv)
{
	char	*path;
	
	if (!argv || !argv[0] || argv[0][0] == '\0')
		return (NULL);
	if (access(argv[0], F_OK) != 0)
		return (NULL);
	else
		return (ft_strdup(argv[0]));
}

char	*find_relative_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path_string;
	char	*full_path;
	int		i;

	if (!cmd || !envp)
		return (NULL);

	i = 0;
	path_string = NULL;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_string = envp[i] + 5; // Skip "PATH="
			break ;
		}
		i++;
	}
	if (!path_string)
		return (NULL);

	paths = ft_split(path_string, ':');
	if (!paths)
		return (NULL);

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin_and_free(full_path, cmd);
		if (!full_path)
		{
			free(full_path);
			free_args(paths);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0)
		{
			free_args(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_args(paths);
	return (NULL);
}

// used for single commands, just wait for one process
int wait_for_child(pid_t pid)
{
    int status;

    waitpid(pid, &status, 0);

    // Print signal messages
    if (WIFSIGNALED(status))
    {
        if (WTERMSIG(status) == SIGQUIT)
            write(1, "Quit (core dumped)\n", 19);
    }
    // Return exit code
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    else
        return (1);
}

/* 

It fills status with extra info:

exit code of the child

whether the child was stopped, continued, killed by a signal, etc.
So we need status to extract the real exit code.

So, if exit(42) was called, the status looks like 42 << 8.
Dividing by 256 (status / 256) shifts it right by 8 bits, giving you the exit code back. 

Example:

child: exit(42)

status = 10752 (because 42 * 256 = 10752)

status / 256 = 42 ✅

status (16 bits):
   [ exit code (8 bits) | flags/signals (8 bits) ]
*/