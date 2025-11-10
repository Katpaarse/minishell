/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:28:40 by jukerste          #+#    #+#             */
/*   Updated: 2025/10/01 15:03:58 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(t_minishell *shell, char const *token)
{
	if (!shell)
		return;
	write(2, "minishell: unexpected token ", 29);
	if (token == NULL)
		write(2, "`newline'", 9);
	else
	{
		write(2, "`", 1);
		write(2, token, ft_strlen(token));
		write(2, "'", 2);
	}
	write(2, "\n", 1);
	shell->exit_code = 258;
}

void	print_error(t_minishell *shell, char const *message)
{
	if (!shell || !message)
		return ;
	write(2, "minishell: ", 11);
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	shell->exit_code = 1;
}

void	print_error_filename(char const	*filename, char	const *message)
{
	if (!filename || !message)
		return ;
	write(2, "minishell: ", 11);
	write(2, filename, ft_strlen(filename));
	write(2, ": ", 2);
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
}
