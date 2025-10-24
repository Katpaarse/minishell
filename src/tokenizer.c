/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:19:46 by jukerste          #+#    #+#             */
/*   Updated: 2025/10/24 17:40:38 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// count how many tokens are in the input string, and it treats special operators (>, >>, <, <<, etc.) as separate tokens
static int	count_tokens(char *input)
{
	int		i;
	int		count;
	char	quote;

	i = 0;
	count = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (input[i] == '\0')
			break;
		count++;
		// Check if this is an operator FIRST
		if (is_special_op(input[i]))
		{
			if ((input[i] == '>' && input[i + 1] == '>') ||
				(input[i] == '<' && input[i + 1] == '<'))
				i = i + 2;
			else
				i++;
		}
		else
		{
			// Handle regular word tokens (can contain quotes)
			while (input[i] && !is_space(input[i]) && !is_special_op(input[i]))
			{
				if (input[i] == '\'' || input[i] == '"')
				{
					quote = input[i];
					i++;
					while (input[i] && input[i] != quote)
						i++;
					if (input[i] == quote)
						i++;
				}
				else
					i++;
			}
		}
	}
	return (count);
}

// splits the input string into an array of strings
char	**tokenize_input(char *input)
{
	int		i;
	int		ti;
	int		start;
	int		num_tokens;
	char	**tokens;
	char	quote;
	
	num_tokens = count_tokens(input);
	tokens = malloc(sizeof(char *) * (num_tokens + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	ti = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (input[i] == '\0')
			break;
		start = i;
		while (input[i] && !is_space(input[i]) && !is_special_op(input[i]))
		{
			if (input[i] == '\'' || input[i] == '"')
			{
				quote = input[i];
				i++; // skip opening quote
				while (input[i] && input[i] != quote)
					i++;
				if (input[i] == quote)
					i++; // skip closing quote
			}
			else
				i++;
		}
		// Special case: if we stopped at an operator, handle it separately
		if (i == start && is_special_op(input[i]))
		{
			if ((input[i] == '>' && input[i + 1] == '>') ||
				(input[i] == '<' && input[i + 1] == '<'))
				i = i + 2;
			else
				i++;
			tokens[ti] = ft_strndup(input + start, i - start);
		}
		else
			tokens[ti] = ft_strndup(input + start, i - start);
		ti++;
	}
	tokens[ti] = NULL;
	return (tokens);
}
