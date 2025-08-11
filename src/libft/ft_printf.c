/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 14:55:36 by jukerste          #+#    #+#             */
/*   Updated: 2025/02/12 14:57:32 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_format_specifier(const char format, va_list args)
{
	void	*ptr;

	if (format == 'c')
		return (ft_putchar((va_arg(args, int))));
	else if (format == 's')
		return (ft_putstr((va_arg(args, char *))));
	else if (format == 'd' || format == 'i')
		return (ft_putnbr((va_arg(args, int))));
	else if (format == 'u')
		return (ft_unsigned_putnbr((va_arg(args, unsigned int))));
	else if (format == 'p')
	{
		ptr = va_arg(args, void *);
		if (ptr != NULL)
			return (ft_putadress((unsigned long)ptr));
		return (ft_putstr("(nil)"));
	}
	else if (format == 'x' || format == 'X')
		return (ft_puthexa(va_arg(args, unsigned int), format));
	else if (format == '%')
		return (ft_putchar('%'));
	else
		return (-1);
}

int	ft_printf(const char *format, ...)
{
	va_list			args;
	unsigned int	count;
	unsigned int	print_length;
	int				result;

	print_length = 0;
	count = 0;
	va_start(args, format);
	while (format[count] != '\0')
	{
		if (format[count] == '%')
		{
			result = ft_format_specifier(format[count + 1], args);
			if (result >= 0)
				print_length = print_length + result;
			else
				return (-1);
			count++;
		}
		else
			print_length = print_length + ft_putchar(format[count]);
		count++;
	}
	va_end(args);
	return (print_length);
}
