/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:31:15 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/01 16:40:24 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// takes an array of tokens (strings) and converts them into a linked list of t_cmd structures, where each node represents a single command, with its arguments and redirections
t_cmd	*tokens_into_cmds(char **tokens)
{
	int		i;
	t_cmd	*current;
	t_cmd	*head;

	if (tokens == NULL || *tokens == NULL) // first check is for pointer to the array is NULL second check to see if the array exists and the first element is NULL. For example when user just presses enter with no input
		return (NULL);
	head = cmd_into_new_node();
	current = head;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '|' && tokens[i][1] == '\0')
		{
			if (current->args == NULL && current->redirects == NULL)
				return (NULL); // syntax error: empty command line before pipe
			current->next = cmd_into_new_node();
			current = current->next;
		}
		else if (tokens[i][0] == '<' && tokens[i][1] == '<' && tokens[i][2] == '\0')
		{
			if (tokens[i + 1] == NULL)
				return (NULL); // syntax error: missing heredoc delimiter
			i++;
			current->redirects = add_redirect(current->redirects, ft_strdup(tokens[i]), RED_HEREDOC); // store delimiter "EOF"
		}
		else if (tokens[i][0] == '<' && tokens[i][1] == '\0')
		{
			if (tokens[i + 1] == NULL)
				return (NULL); // syntax error: missing infile 
			i++;
			current->redirects = add_redirect(current->redirects, ft_strdup(tokens[i]), RED_INPUT);
		}
		else if (tokens[i][0] == '>' && tokens[i][1] == '\0')
		{
			if (tokens[i + 1] == NULL)
				return (NULL); // syntax error: missing outfile
			i++;
			current->redirects = add_redirect(current->redirects, ft_strdup(tokens[i]), RED_OUTPUT);
		}
		else if (tokens[i][0] == '>' && tokens[i][1] == '>' && tokens[i][2] == '\0')
		{
			if (tokens[i + 1] == NULL)
				return (NULL); // syntax error: missing outfile
			i++;
			current->redirects = add_redirect(current->redirects, ft_strdup(tokens[i]), RED_APPEND);
		}
		else
			current->args = add_argument(current->args, ft_strdup(tokens[i]));
		i++;
	}
	return (head);
}
