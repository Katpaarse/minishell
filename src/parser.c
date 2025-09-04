/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:31:15 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/04 16:36:53 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// takes an array of tokens (strings) and converts them into a linked list of t_cmd structures, where each node represents a single command, with its arguments and redirections
t_cmd	*tokens_into_cmds(char **tokens, t_minishell *shell)
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
			{
				print_syntax_error(shell, tokens[i]);
				return (NULL);
			}
			current->next = cmd_into_new_node();
			current = current->next;
		}
		else if (tokens[i][0] == '<' && tokens[i][1] == '<' && tokens[i][2] == '\0')
		{
			if (tokens[i + 1] == NULL)
			{
				print_syntax_error(shell, NULL); // syntax error: missing heredoc delimiter
				return (NULL); 
			}
			i++;
			current->redirects = add_redirect(current->redirects, ft_strdup(tokens[i]), RED_HEREDOC); // store delimiter "EOF"
		}
		else if (tokens[i][0] == '<' && tokens[i][1] == '\0')
		{
			if (tokens[i + 1] == NULL)
			{
				print_syntax_error(shell, NULL); // syntax error: missing infile
				return (NULL);  
			}
			i++;
			current->redirects = add_redirect(current->redirects, ft_strdup(tokens[i]), RED_INPUT);
		}
		else if (tokens[i][0] == '>' && tokens[i][1] == '\0')
		{
			if (tokens[i + 1] == NULL)
			{
				print_syntax_error(shell, NULL); // syntax error: missing outfile
				return (NULL);
			}
			i++;
			current->redirects = add_redirect(current->redirects, ft_strdup(tokens[i]), RED_OUTPUT);
		}
		else if (tokens[i][0] == '>' && tokens[i][1] == '>' && tokens[i][2] == '\0')
		{
			if (tokens[i + 1] == NULL)
			{
				print_syntax_error(shell, NULL); // syntax error: missing outfile
				return (NULL);
			}
			i++;
			current->redirects = add_redirect(current->redirects, ft_strdup(tokens[i]), RED_APPEND);
		}
		else
			current->args = add_argument(current->args, ft_strdup(tokens[i]));
		i++;
	}
	return (head);
}
