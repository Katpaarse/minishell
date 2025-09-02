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

char	*find_cmd_path(t_cmd *cmd, t_minishell *shell)
{
	char	*path;

	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0' || !shell->envp)
		return (NULL);

	if (ft_strchr(cmd->args[0], '/') != NULL)
		return (find_absolute_path(cmd));
	else
		return (find_relative_path(cmd, shell));

	return (NULL);
}

char	*find_absolute_path(t_cmd *cmd)
{
	int		i;
	char	*path;

	i = 0;
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
		return (NULL);

	if (access(cmd->args[0], X_OK) == 0)
		return (cmd->args[0]);
	else
	{
		// error handling can be added here if needed
		return (NULL);
	}
}

char	*find_relative_path(t_cmd *cmd, t_minishell *shell)
{
	char	**paths;
	char	*path_string;
	char	*full_path;
	int		i;

	if (!cmd->args || !shell->envp)
		return (NULL);

	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "PATH=", 5) == 0)
		{
			path_string = shell->envp[i] + 5; // Skip "PATH="
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
		full_path = ft_strjoin_and_free(full_path, cmd->args[0]);
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

