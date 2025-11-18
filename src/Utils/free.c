/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 14:08:07 by lavan-de          #+#    #+#             */
/*   Updated: 2025/11/18 14:02:26 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirects(t_redirect *redirect)
{
	t_redirect	*tmp;

	while (redirect)
	{
		tmp = redirect->next;
		free(redirect->filename);
		free(redirect);
		redirect = tmp;
	}
}

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_args(cmd->args);
		free_redirects(cmd->redir);
		free(cmd);
		cmd = tmp;
	}
}

void	free_minishell(t_minishell *shell)
{
	if (!shell)
		return ;
	if (shell->cmds != NULL)
		free_cmds(shell->cmds);
	shell->cmds = NULL;
	if (shell->envp != NULL)
		free_args(shell->envp);
	shell->envp = NULL;
	if (shell->exp_list != NULL)
		free_args(shell->exp_list);
	shell->exp_list = NULL;
}

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
