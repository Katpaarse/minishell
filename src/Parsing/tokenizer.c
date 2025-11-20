/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:19:46 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/20 15:20:02 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_tokens(char *input)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (input[i])
	{
		i = skip_spaces(input, i);
		if (!input[i])
			break ;
		count++;
		if (is_special_op(input[i]))
			i = skip_special_operator(input, i);
		else
		{
			while (input[i] && !is_space(input[i]) && !is_special_op(input[i]))
			{
				if (input[i] == '\'' || input[i] == '"')
					i = skip_quoted_input(input, i);
				else
					i++;
			}
		}
	}
	return (count);
}

static char	*extract_token(char *input, int *i)
{
	int	start;

	start = *i;
	if (is_special_op(input[*i]))
		*i = skip_special_operator(input, *i);
	else
	{
		while (input[*i] && !is_space(input[*i]) && !is_special_op(input[*i]))
		{
			if (input[*i] == '\'' || input[*i] == '"')
				*i = skip_quoted_input(input, *i);
			else
				(*i)++;
		}
	}
	return (ft_strndup(input + start, *i - start));
}

/* Splits the input line into an array of tokens for the parser. */
char	**tokenize_input(char *input)
{
	int		i;
	int		ti;
	int		num_tokens;
	char	**tokens;

	num_tokens = count_tokens(input);
	tokens = malloc(sizeof(char *) * (num_tokens + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	ti = 0;
	while (input[i])
	{
		i = skip_spaces(input, i);
		if (!input[i])
			break ;
		tokens[ti++] = extract_token(input, &i);
	}
	tokens[ti] = NULL;
	return (tokens);
}
