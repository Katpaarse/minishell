/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:01 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/25 17:25:17 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*change_dir(t_cmd *cmd, t_minishell *shell, char *old_pwd)
{
	char	*path;

	if (!shell)
		return (NULL);
	if (cmd->args[1] == NULL)
		path = get_env_value("HOME", shell->envp);
	else if (ft_strncmp(cmd->args[1], "-", 2) == 0)
		path = get_env_value("OLDPWD", shell->envp);
	else
		path = cmd->args[1];
	if (!path)
	{
		print_error(shell, "cd: HOME/OLDPWD not set");
		free(old_pwd);
		return (NULL);
	}
	if (chdir(path) != 0)
	{
		print_error(shell, "cd: No such file or directory");
		free(old_pwd);
		return (NULL);
	}
	return (path);
}

void	set_pwd_env(t_cmd *cmd, t_minishell *shell, char *n_p, char *o_p)
{
	int	pwd_i;
	int	i;

	if (!shell)
		return ;
	pwd_i = -1;
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "PWD=", 4) == 0 && n_p)
		{
			free(shell->envp[i]);
			shell->envp[i] = ft_strjoin("PWD=", n_p);
			pwd_i = i;
		}
		else if (ft_strncmp(shell->envp[i], "OLDPWD=", 7) == 0 && o_p)
		{
			free(shell->envp[i]);
			shell->envp[i] = ft_strjoin("OLDPWD=", o_p);
		}
		i++;
	}
	if (cmd->args[1] && ft_strncmp(cmd->args[1], "-", 2) == 0 && pwd_i != -1)
	{
		write(1, shell->envp[pwd_i], ft_strlen(shell->envp[pwd_i]));
		write(1, "\n", 1);
	}
}

int	builtin_cd(t_cmd *cmd, t_minishell *shell)
{
	char	*path;
	char	*new_pwd;
	char	*old_pwd;

	if (!cmd || !cmd->args || !shell)
		return (FAILURE);
	if (cmd->args[1] && cmd->args[2])
	{
		print_error(shell, "cd: too many arguments");
		return (FAILURE);
	}
	new_pwd = NULL;
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		old_pwd = NULL;
	path = change_dir(cmd, shell, old_pwd);
	if (!path)
		return (FAILURE);
	if (old_pwd)
	{
		new_pwd = getcwd(NULL, 0);
		if (!new_pwd)
		{
			free(old_pwd);
			return (FAILURE);
		}
	}
	else if (!old_pwd && !new_pwd)
		new_pwd = path;
	set_pwd_env(cmd, shell, new_pwd, old_pwd);
	return (SUCCESS);
}
