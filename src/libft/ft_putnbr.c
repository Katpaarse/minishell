/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:26:57 by jukerste          #+#    #+#             */
/*   Updated: 2025/02/12 14:57:53 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr(int nb)
{
	int	count;

	count = 0;
	if (nb < 0)
	{
		if (nb == -2147483648)
			return (write(1, "-2147483648", 11));
		else
		{
			count = count + ft_putchar('-');
			nb = nb * -1;
		}
	}
	if (nb > 9)
		count = count + ft_putnbr(nb / 10);
	count = count + ft_putchar((nb % 10) + '0');
	return (count);
}

int	ft_unsigned_putnbr(unsigned int nb)
{
	int	count;

	count = 0;
	if (nb > 9)
		count = count + ft_putnbr(nb / 10);
	count = count + ft_putchar((nb % 10) + '0');
	return (count);
}
