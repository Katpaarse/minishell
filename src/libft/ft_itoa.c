/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:20:02 by jukerste          #+#    #+#             */
/*   Updated: 2024/10/30 15:57:19 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_digits(int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		i++;
	while (n != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

static char	*special_cases(int n)
{
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	return (0);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	int		i;
	int		num;

	if (n == -2147483648 || n == 0)
		return (special_cases(n));
	len = count_digits(n);
	str = malloc((len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	num = n;
	if (n < 0)
		n = n * -1;
	i = len;
	str[i--] = '\0';
	while (n > 0)
	{
		str[i] = n % 10 + '0';
		n = n / 10;
		i--;
	}
	if (num < 0)
		str[i] = '-';
	return (str);
}
