/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putadress.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:41:13 by jukerste          #+#    #+#             */
/*   Updated: 2025/02/12 14:57:38 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putadress(unsigned long ptr)
{
	char	str[100];
	char	*base;
	int		count;
	int		i;

	i = 0;
	count = 0;
	base = "0123456789abcdef";
	if (ptr == 0)
		count = count + write(1, "(nil)", 5);
	if (ptr != 0)
	{
		count = count + write(1, "0x", 2);
		while (ptr != 0)
		{
			str[i++] = base[ptr % 16];
			ptr = ptr / 16;
		}
		while (i > 0)
		{
			i--;
			count = count + write(1, &str[i], 1);
		}
	}
	return (count);
}
