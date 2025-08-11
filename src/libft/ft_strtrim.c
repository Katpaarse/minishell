/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:05:12 by jukerste          #+#    #+#             */
/*   Updated: 2024/10/30 15:59:39 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	len;
	char	*str;

	len = ft_strlen(s1);
	i = 0;
	while (0 < len && ft_strrchr(set, s1[len - 1]))
		len--;
	while (i < len && ft_strrchr(set, s1[i]))
		i++;
	len = len - i + 1;
	str = malloc(len);
	if (str == NULL || set == NULL)
		return (NULL);
	ft_strlcpy(str, (char *) s1 + i, len);
	return (str);
}
