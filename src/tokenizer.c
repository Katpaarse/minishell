/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:19:46 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/17 14:43:09 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}
static int	is_special_op(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

static int	count_tokens(char *input)
{
	int	i;
	int	count;

	i = 0;
	count = 0;

	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (input[i] == '\0')
			break;
		count++;
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
			while (input[i] && !is_space(input[i]) && !is_special_op(input[i]))
			i++;
		}
	}
	return (count);
}
char	**tokenize_input(char *input)
{
	int		i;
	int		ti; // token output || token index
	int		start;
	int		num_tokens;
	char	**tokens;
	
	num_tokens = count_tokens(input);
	tokens = malloc(sizeof(char *) * (num_tokens + 1));
	if (tokens == NULL)
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
			while (input[i] && !is_space(input[i]) && !is_special_op(input[i]))
				i++;
		}
		tokens[ti] = ft_strndup(input + start, i - start);
		ti++;
	}
	tokens[ti] = NULL;
	return (tokens);
}
