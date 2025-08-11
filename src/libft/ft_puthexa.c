/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthexa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 19:12:51 by jukerste          #+#    #+#             */
/*   Updated: 2025/02/12 14:57:46 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_hexlen(unsigned int nb)
{
	int	len;

	len = 0;
	while (nb != 0)
	{
		len++;
		nb = nb / 16;
	}
	return (len);
}

void	ft_hexconverter(unsigned int nb, const char format)
{
	if (nb >= 16)
	{
		ft_hexconverter(nb / 16, format);
		ft_hexconverter(nb % 16, format);
	}
	else
	{
		if (nb <= 9)
			ft_putchar((nb + '0'));
		else
		{
			if (format == 'x')
				ft_putchar((nb - 10 + 'a'));
			if (format == 'X')
				ft_putchar((nb - 10 + 'A'));
		}
	}
}

int	ft_puthexa(unsigned int nb, const char format)
{
	if (nb == 0)
		return (write(1, "0", 1));
	else
		ft_hexconverter(nb, format);
	return (ft_hexlen(nb));
}
