/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jul <jul@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 16:37:59 by lavan-de          #+#    #+#             */
/*   Updated: 2025/10/24 20:47:59 by jul              ###   ########.fr       */
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
	if (!argv || !argv[0] || argv[0][0] == '\0')
		return (NULL);
	if (access(argv[0], F_OK) != 0)
		return (NULL);
	else
		return (ft_strdup(argv[0]));
}

char	**find_path(char **envp, char **paths)
{
	int		i;
	char	*path_string;

	i = 0;
	path_string = NULL;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_string = envp[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_string)
		return (NULL);
	paths = ft_split(path_string, ':');
	if (!paths)
		return (NULL);
	return (paths);
}

char	*find_relative_path(char *cmd, char **envp)
{
	char	**paths;
	char	*full_path;
	int		i;

	paths = NULL;
	paths = find_path(envp, paths);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin_and_free(full_path, cmd);
		if (!full_path)
			return (free(full_path), free_args(paths), NULL);
		if (access(full_path, X_OK) == 0)
			return (free_args(paths), full_path);
		free(full_path);
		i++;
	}
	free_args(paths);
	return (NULL);
}

int	wait_for_child(pid_t pid)
{
	int	status;
	int	exit_code;
	int	signal_number;

	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno != EINTR)
			return (1);
	}
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		signal_number = WTERMSIG(status);
		exit_code = 128 + signal_number;
		if (signal_number == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
	}
	else
		exit_code = 1;
	return (exit_code);
}
