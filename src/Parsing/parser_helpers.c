/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:03:17 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/20 16:55:55 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_redirect_type(char *token)
{
	if (token[0] == '<')
		return (RED_INPUT);
	else if (token[0] == '>' && token[1] == '\0')
		return (RED_OUTPUT);
	else
		return (RED_APPEND);
}

/* Toggles single/double quote states while scanning a string.
Used to track whether the parser is inside quotes. */
void	update_quotes(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '"' && !(*in_single))
		*in_double = !(*in_double);
}

/* function to see if a '|' char is encountered and 
creates a new command node */
int	handle_pipe_token(t_parsing *p)
{
	t_cmd	*next_node;

	if (!p->current->args && !p->current->redir)
	{
		print_syntax_error(p->shell, "|");
		return (-1);
	}
	next_node = cmd_into_new_node();
	if (!next_node)
		return (-1);
	p->current->next = next_node;
	p->current = next_node;
	return (0);
}

/* Handles <, >, and >> tokens: determines redirect type,
fetches the filename, and appends a redirect node
to the current command.*/
int	handle_redirect_token(t_parsing *p)
{
	int			type;
	char		*token;
	char		*filename;
	t_redirect	*new_list;

	token = p->tokens[p->i];
	if (!token)
	{
		print_syntax_error(p->shell, NULL);
		return (-1);
	}
	type = get_redirect_type(token);
	(p->i)++;
	if (!p->tokens[p->i])
		return (print_syntax_error(p->shell, NULL), -1);
	filename = ft_strdup(p->tokens[p->i]);
	if (!filename)
		return (-1);
	new_list = add_redirect(p->current->redir, filename, type);
	if (!new_list)
		return (-1);
	p->current->redir = new_list;
	return (0);
}

/* Process a << heredoc: create tmpfile, handle errors, and
update parser indexes (i and hi) */
int	handle_heredoc_wrapper(t_parsing *p)
{
	int		result;
	char	*delimiter;

	delimiter = p->tokens[p->i + 1];
	result = handle_heredoc_token(p->current,
			p->tokens[p->i + 1], p->hi, p->shell);
	if (result == -2)
	{
		p->shell->exit_code = 130;
		return (-1);
	}
	if (result != 0)
		return (-1);
	(p->i)++;
	(p->hi)++;
	return (0);
}
