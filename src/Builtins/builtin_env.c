/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:07 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/18 18:59:44 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
    env: env with no options or arguments requires 0 arguments.
        Example: env
*/

int	builtin_env(t_minishell *shell, int fd)
{
	int	i;

	i = 0;
	while (shell->envp[i] != NULL)
	{
		write(fd, shell->envp[i], ft_strlen(shell->envp[i]));
		write(fd, "\n", 1); // Print newline after each
		i++;
	}
	return (0);
}
