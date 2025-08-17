/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:31:15 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/17 15:29:11 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// takes an array of tokens (strings) and converts them into a linked list of t_cmd structures, where each node represents a single command, with its arguments and redirections
t_cmd	*tokens_into_cmds(char **tokens)
{
	int		i;
	t_cmd	*current;
	t_cmd	*head;

	head = cmd_into_new_node();
	current = head;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '|' && tokens[i][1] == '\0')
		{
			current->next = cmd_into_new_node();
			current = current->next;
		}
		else if (tokens[i][0] == '<' && tokens[i][1] == '\0')
		{
			i++;
			current->infile = ft_strdup(tokens[i]);
		}
		else if (tokens[i][0] == '>' && tokens[i][1] == '\0')
		{
			i++;
			current->outfile = ft_strdup(tokens[i]);
			current->append = FALSE;
		}
		else if (tokens[i][0] == '>' && tokens[i][1] == '>' && tokens[i][2] == '\0')
		{
			i++;
			current->outfile = ft_strdup(tokens[i]);
			current->append = TRUE;
		}
		else
			current->args = add_argument(current->args, ft_strdup(tokens[i]));
		i++;
	}
	return (head);
}
