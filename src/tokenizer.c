/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:19:46 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/14 19:33:53 by jukerste         ###   ########.fr       */
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
	
}
