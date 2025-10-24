/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:31:15 by jukerste          #+#    #+#             */
/*   Updated: 2025/10/24 17:38:45 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredoc_token(t_cmd *current, char *raw_token, int heredoc_index, t_minishell *shell)
{
	int		expand;
	size_t	len;
	char	*tmpfile;
	char	*clean_delim;
	
	if (!raw_token)
	{
		print_syntax_error(shell, NULL);
		return (-1);
	}
	len = ft_strlen(raw_token);
	expand = 1;
	if ((raw_token[0] == '\'' && raw_token[len - 1] == '\'') ||
		(raw_token[0] == '"' && raw_token[len - 1] == '"'))
		expand = 0;
	clean_delim = remove_quotes(raw_token);
	tmpfile = process_heredoc(clean_delim, heredoc_index, shell, expand);
	free(clean_delim);
	if (!tmpfile)
	{
		print_syntax_error(shell, NULL);		
		return (-1);
	}
	current->redirects = add_redirect(current->redirects, tmpfile, RED_HEREDOC, expand);
	return (0);
}

static int	handle_redirect_token(t_cmd *current, char **tokens, int *i, t_minishell *shell)
{
	int	type;
	
	if (!tokens[*i])
	{
		print_syntax_error(shell, NULL);
		return (-1);
	}
	if (tokens[*i][0] == '<')
		type = RED_INPUT;
	else if (tokens[*i][0] == '>' && tokens[*i][1] == '\0')
		type = RED_OUTPUT;
	else
		type = RED_APPEND;
	(*i)++;
	if (!tokens[*i])
	{
		print_syntax_error(shell, NULL);
		return (-1);
	}
	current->redirects = add_redirect(current->redirects, ft_strdup(tokens[*i]), type, 0);
	return (0);
}

static t_cmd	*handle_pipe_token(t_cmd *current, t_minishell *shell)
{
	if (!current->args && !current->redirects)
	{
		print_syntax_error(shell, "|");
		return (NULL);
	}
	current->next = cmd_into_new_node();
	return (current->next);
}

// takes an array of tokens (strings) and converts them into a linked list of t_cmd structures, where each node represents a single command, with its arguments and redirections
t_cmd	*tokens_into_cmds(char **tokens, t_minishell *shell)
{
	t_cmd	*current;
	t_cmd	*head;
	int		i;
	int		heredoc_index;

	if (!tokens || !*tokens) // first check is for pointer to the array is NULL second check to see if the array exists and the first element is NULL. For example when user just presses enter with no input
		return (NULL);
	head = cmd_into_new_node();
	current = head;
	heredoc_index = 1; // unique counter for heredocs
	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '|' && tokens[i][1] == '\0')
		{
			current = handle_pipe_token(current, shell);
			if (!current)
			{
				free_cmds(head);
				return (NULL);
			}
		}
		else if (tokens[i][0] == '<' && tokens[i][1] == '<' && tokens[i][2] == '\0')
		{
			if (handle_heredoc_token(current, tokens[i + 1], heredoc_index, shell) != 0)
			{
				free_cmds(head);
				return NULL;
			}
			i++;
			heredoc_index++;
		}
		else if ((tokens[i][0] == '<') ||
				(tokens[i][0] == '>' && tokens[i][1] == '\0') ||
				(tokens[i][0] == '>' && tokens[i][1] == '>' && tokens[i][2] == '\0'))
		{
			if (handle_redirect_token(current, tokens, &i, shell) != 0)
			{
				free_cmds(head);
				return NULL;
			}
		}
		else
			current->args = add_argument(current->args, ft_strdup(tokens[i]));
		i++;
	}
	if (!current->args && !current->redirects) // final check: empty last command after a pipe
	{
		print_syntax_error(shell, NULL);
		free_cmds(head);
		return (NULL);
	}
	return (head);
}
