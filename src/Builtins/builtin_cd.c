/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd(NOT DONE).c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:01 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/21 19:35:43 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
    cd: cd with a relative or absolute path requires exactly 1 argument. 
	The path is that argument.
        Example: cd /home, cd ../documents
*/

int	builtin_cd(t_cmd *cmd, t_minishell *shell)
{
	char	*path;
	char	*oldpwd;
	int 	i;

	if (!cmd || !cmd->args || !shell)
		return (FAILURE);

	if (cmd->args[2] != NULL)
	{
		write(2, "cd: too many arguments\n", 24);
		return (FAILURE);
	}

	oldpwd = getcwd(NULL, 0); // MALLOC so free it later
	if (!oldpwd)
		return (FAILURE);

	if (cmd->args[1] == NULL)
		path = getenv("HOME");
	else if (ft_strncmp(cmd->args[1], "-", 2) == 0)
		path = getenv("OLDPWD");
	else
		path = cmd->args[1];

	if (!path)
	{
		write(2, "cd: HOME not set\n", 17);
		free(oldpwd);
		return (FAILURE);
	}

	if (path && chdir(path) == 0)
	{
		write(1, path, ft_strlen(path));
		write(1, "\n", 1);
	}
	else if (path && chdir(path) != 0)
	{
		write(2, "cd: no such file or directory\n", 31);
		free(oldpwd);
		return (FAILURE);
	}
	else
		return (FAILURE);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "PWD=", 4) == 0)
		{
			free(shell->envp[i]);
			shell->envp[i] = ft_strjoin_and_free(ft_strdup("PWD="), getcwd(NULL, 0));
		}
		else if (ft_strncmp(shell->envp[i], "OLDPWD=", 7) == 0)
		{
			free(shell->envp[i]);
			shell->envp[i] = ft_strjoin_and_free(ft_strdup("OLDPWD="), oldpwd);
		}
		i++;
	}

	return (SUCCESS);
}
