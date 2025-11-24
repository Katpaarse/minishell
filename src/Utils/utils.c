/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:58:19 by lavan-de          #+#    #+#             */
/*   Updated: 2025/11/24 13:43:34 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_whitespace_only(char *str)
{
	if (!str)
		return (FALSE);
	while (*str)
	{
		if (!is_space(*str))
			return (FALSE);
		str++;
	}
	return (TRUE);
}

char	*ft_strjoin_and_free(char *s1, char *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*result;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	result = malloc(len_s1 + len_s2 + 1);
	if (!result)
	{
		free(s1);
		return (NULL);
	}
	ft_memcpy(result, s1, len_s1);
	ft_memcpy(result + len_s1, s2, len_s2);
	result[len_s1 + len_s2] = '\0';
	free(s1);
	return (result);
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_special_op(char c)
{
	return (c == '|' || c == '>' || c == '<');
}
