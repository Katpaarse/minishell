/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 13:30:22 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/18 13:44:38 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_shell_cmds(t_minishell *shell)
{
	if (!shell || !shell->cmds)
		return ;
	cleanup_heredoc_files(shell->cmds->redir);
	free_cmds(shell->cmds);
	shell->cmds = NULL;
}

static int	alloc_env_arrays(t_minishell *shell, size_t count)
{
	shell->exp_list = malloc(sizeof(char *) * (count + 1));
	if (!shell->exp_list)
		return (0);
	shell->envp = malloc(sizeof(char *) * (count + 1));
	if (!shell->envp)
	{
		free(shell->exp_list);
		shell->exp_list = NULL;
		return (0);
	}
	return (1);
}

static int	duplicate_env_entry(t_minishell *shell, char **envp, int i)
{
	shell->exp_list[i] = ft_strdup(envp[i]);
	shell->exp_list[i + 1] = NULL;
	if (!shell->exp_list[i])
		return (0);
	shell->envp[i] = ft_strdup(envp[i]);
	shell->envp[i + 1] = NULL;
	if (!shell->envp[i])
		return (0);
	return (1);
}

void	copy_envp(t_minishell *shell, char **envp)
{
	size_t	count;
	int		i;

	if (!envp || !shell)
		return ;
	count = count_env(envp);
	if (!alloc_env_arrays(shell, count))
		return ;
	i = 0;
	while (envp[i])
	{
		if (!duplicate_env_entry(shell, envp, i))
		{
			free_args(shell->exp_list);
			free_args(shell->envp);
			shell->exp_list = NULL;
			shell->envp = NULL;
			return ;
		}
		i++;
	}
}
