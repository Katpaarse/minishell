/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 18:04:27 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/11 15:11:21 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*freesubstr(char **array, int j)
{
	int	i;

	i = 0;
	while (i < j)
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (NULL);
}

static int	count_words(char const *s, char c)
{
	int	words;
	int	i;
	int	reset;

	i = 0;
	reset = 0;
	words = 0;
	while (s[i])
	{
		if (s[i] == c)
			reset = 0;
		if (s[i] != c && reset == 0)
		{
			words++;
			reset = 1;
		}
		i++;
	}
	return (words);
}

char	**ft_split(char const *s, char c)
{
	int		start;
	int		i;
	int		j;
	char	**array;

	array = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (array == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (j < count_words(s, c))
	{
		while (s[i] && s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		array[j] = ft_substr(s, start, i - start);
		if (array[j] == NULL)
			return (freesubstr(array, j));
		j++;
	}
	array[j] = NULL;
	return (array);
}
