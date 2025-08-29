/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:28:40 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/29 20:18:17 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(char const *source, char const *message, char const *token)
{
	if (source == NULL || message == NULL || token == NULL)
		return;
	write(2, source, ft_strlen(source));
	write(2, ": ", 2);
	write(2, message, ft_strlen(message));
	write(2, " '", 2);
	write(2, token, ft_strlen(token));
	write(2, "'\n", 2);
}
