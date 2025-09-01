/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 16:37:59 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/31 16:38:00 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*find_cmd_path(char **argv, char **envp)
{
	char	*path;

	if (!argv || !argv[0] || argv[0][0] == '\0' || !envp)
		return (NULL);

	if (ft_strchr(argv[0], '/') != NULL)
		return (find_absolute_path(argv));
	else
		return (find_relative_path(argv[0], envp));

	return (NULL);
}

char	*find_absolute_path(char **argv)
{
	int		i;
	char	*path;

	i = 0;
	if (!argv || !argv[0] || argv[0][0] == '\0')
		return (NULL);

	if (access(argv[0], X_OK) == 0)
		return (argv[0]);
	else
	{
		// error handling can be added here if needed
		return (NULL);
	}
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
	while (envp[i])
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
		if (access(full_path, X_OK) == 0)
		{
			// Free allocated memory for paths
			return (full_path);
		}
		// Free allocated memory for paths
		free(full_path);
		i++;
	}
	// Free allocated memory for paths
	return (NULL);
}

int wait_for_child(pid_t pid)
{
    int status;

    waitpid(pid, &status, 0);
    return (status);
}

