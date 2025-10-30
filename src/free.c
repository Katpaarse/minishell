/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 14:08:07 by lavan-de          #+#    #+#             */
/*   Updated: 2025/10/30 14:08:16 by lavan-de         ###   ########.fr       */
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

void free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next; // save next node
		free_args(cmd->args); // free arguments array
		free_redirects(cmd->redirects); // free redirects list
		free(cmd); // free command node
		cmd = tmp; // move to next node
	}
}

void free_minishell(t_minishell *shell)
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