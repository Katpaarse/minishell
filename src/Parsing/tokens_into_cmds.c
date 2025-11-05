/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_into_cmds.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:31:15 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/05 17:43:34 by jukerste         ###   ########.fr       */
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

static int	process_token(t_parsing *p)
{
	char *token;

	token = p->tokens[*(p->i)];

	if (is_pipe_token(token))
		return (handle_pipe_token(p));
	if (is_heredoc_token(token))
		return (handle_heredoc_wrapper(p));
	if (is_redirect_token(token))
		return (handle_redirect_token(p));
	if (token[0] == '\0' && !(*(p->current))->args)
		return (0);
	(*(p->current))->args = add_argument((*(p->current))->args, ft_strdup(token));
	return (0);
}

t_cmd	*tokens_into_cmds(char **tokens, t_minishell *shell)
{
	t_cmd		*head;
	t_cmd		*current;
	t_parsing	p;
	int			i;
	int			heredoc_index;

	if (!tokens || !*tokens)
		return (NULL);
	head = cmd_into_new_node();
	current = head;
	heredoc_index = 1;
	i = 0;
	p.current = &current;
	p.tokens = tokens;
	p.i = &i;
	p.hi = &heredoc_index;
	p.shell = shell;

	while (tokens[i])
	{
		if (process_token(&p) != 0)
		{
			free_cmds(head);
			return (NULL);
		}
		i++;
	}
	if (!current->args && !current->redir)
	{
		free_cmds(head);
		return (NULL);
	}
	return (head);
}
