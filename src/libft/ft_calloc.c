/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:41:39 by jukerste          #+#    #+#             */
/*   Updated: 2024/10/30 15:56:41 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	i;
	char	*str;

	if (nmemb && size > __SIZE_MAX__ / nmemb)
		return (NULL);
	str = malloc(size * nmemb);
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < (size * nmemb))
	{
		str[i] = 0;
		i++;
	}
	return (str);
}
