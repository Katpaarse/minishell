/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:19:46 by jukerste          #+#    #+#             */
/*   Updated: 2025/10/06 18:44:24 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}
int	is_special_op(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

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
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i];
			i++;
			while (input[i] && input[i] != quote)
				i++;
			if (input[i] == '\0')
				return (-1); // unclosed quote
			i++;
		}
		else if (is_special_op(input[i]))
		{
			if ((input[i] == '>' && input[i + 1] == '>') ||
				(input[i] == '<' && input[i + 1] == '<'))
				i = i + 2;
			else
				i++;
		}
		else
		{
			while (input[i] && !is_space(input[i]) && !is_special_op(input[i]) && input[i] != '\'' && input[i] != '"')
				i++;
		}
	}
	return (count);
}

// splits the input string into an array of strings
char	**tokenize_input(char *input)
{
	int		i;
	int		ti; // token output || token index
	int		start;
	int		num_tokens;
	char	**tokens;
	char	quote;
	
	num_tokens = count_tokens(input);
	if (num_tokens == -1) // unclosed quote
		return (NULL);
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
		else if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i];
			start = i;
			i++;
			while (input[i] && input[i] != quote)
				i++;
			if (input[i] == quote)
				i++;
			tokens[ti] = ft_strndup(input + start, i - start);
			ti++;
		}
		else if (is_special_op(input[i]))
		{
			start = i;
			if ((input[i] == '>' && input[i + 1] == '>') ||
				(input[i] == '<' && input[i + 1] == '<'))
				i = i + 2;
			else
				i++;
			tokens[ti] = ft_strndup(input + start, i - start);
			ti++;
		}
		else
		{
			start = i;
			while (input[i] && !is_space(input[i]) && !is_special_op(input[i]) && input[i] != '\'' && input[i] != '"')
				i++;
			tokens[ti] = ft_strndup(input + start, i - start);
			ti++;
		}
	}
	tokens[ti] = NULL;
	return (tokens);
}
