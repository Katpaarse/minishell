/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:03:17 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/05 17:42:42 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pipe_token(t_parsing *p)
{
	if (!(*(p->current))->args && !(*(p->current))->redir)
		return (print_syntax_error(p->shell, "|"), -1);
	(*(p->current))->next = cmd_into_new_node();
	*(p->current) = (*(p->current))->next;
	return (0);
}

int	handle_redirect_token(t_parsing *p)
{
	int	type;
	char *token;

	token = p->tokens[*(p->i)];

	if (!token)
		return (print_syntax_error(p->shell, NULL), -1);

	if (token[0] == '<')
		type = RED_INPUT;
	else if (token[0] == '>' && token[1] == '\0')
		type = RED_OUTPUT;
	else
		type = RED_APPEND;

	(*(p->i))++;

	if (!p->tokens[*(p->i)])
		return (print_syntax_error(p->shell, NULL), -1);

	(*(p->current))->redir = add_redirect((*(p->current))->redir,
			ft_strdup(p->tokens[*(p->i)]),
			type,
			0);
	return (0);
}

int	handle_heredoc_wrapper(t_parsing *p)
{
	int	result;

	result = handle_heredoc_token(*(p->current),
			p->tokens[*(p->i) + 1],
			*(p->hi),
			p->shell);

	if (result == -2)
	{
		p->shell->exit_code = 130;
		return (-1);
	}
	if (result != 0)
		return (-1);

	(*(p->i))++;
	(*(p->hi))++;
	return (0);
}
