/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:17 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/18 18:59:44 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
    unset: unset with no options requires 0 arguments.
        Example: unset
*/

int	builtin_unset(t_cmd *cmd, t_minishell *shell)
{
	int i;

	i = 1;
	while (cmd->args[i] != NULL)
	{
		if (cmd->args[i][0] == '\0')
		{
			fprintf(stderr, "unset: `%s': not a valid identifier\n", cmd->args[i]);
			return (FAILURE);
		}

		i++;
	}
	return (SUCCESS);
}
