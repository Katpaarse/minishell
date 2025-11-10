/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_into_cmds.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:31:15 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/06 13:37:20 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_pipe_token(char *str)
{
	return (str[0] == '|' && str[1] == '\0');
}

static int	is_heredoc_token(char *str)
{
	return (str[0] == '<' && str[1] == '<' && str[2] == '\0');
}

static int	is_redirect_token(char *str)
{
	return ((str[0] == '<' && str[1] == '\0') ||
		(str[0] == '>' && str[1] == '\0') ||
		(str[0] == '>' && str[1] == '>' && str[2] == '\0'));
}

static int	handle_token(t_parsing *p)
{
	char	*token;
	char	*token_copy;

	token = p->tokens[p->i];
	if (is_pipe_token(token))
		return (handle_pipe_token(p));
	if (is_heredoc_token(token))
		return (handle_heredoc_wrapper(p));
	if (is_redirect_token(token))
		return (handle_redirect_token(p));
	if (token[0] == '\0' && !(p->current)->args)
		return (0);
	token_copy = ft_strdup(token);
	if (!token_copy)
		return (-1);
	p->current->args = add_argument(p->current->args, token_copy);
	if (!p->current->args)
		return (-1);
	return (0);
}

t_cmd	*tokens_into_cmds(char **tokens, t_minishell *shell)
{
	t_cmd		*head;
	t_parsing	p;

	if (!tokens || !*tokens)
		return (NULL);
	head = cmd_into_new_node();
	if (!head)
		return (NULL);
	p.current = head;
	p.tokens = tokens;
	p.i = 0;
	p.hi = 1;
	p.shell = shell;

	while (p.tokens[p.i])
	{
		if (handle_token(&p) != 0)
			return (free_cmds(head), NULL);
		p.i++;
	}
	if (!p.current->args && !p.current->redir)
		return (free_cmds(head), NULL);
	return (head);
}
