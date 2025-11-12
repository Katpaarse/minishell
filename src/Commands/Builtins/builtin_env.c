/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:07 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/21 19:35:56 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_minishell *shell)
{
	int	i;

	if (!shell->envp)
		return (FAILURE);
	i = 0;
	while (shell->envp[i] != NULL)
	{
		write(1, shell->envp[i], ft_strlen(shell->envp[i]));
		write(1, "\n", 1);
		i++;
	}
	return (SUCCESS);
}
