/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export(NOT DONE).c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:11 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/21 19:36:07 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ft_strncmp
// ft_strlen
// malloc/free
// write
/*
    export: export with no options requires 0 arguments.
        Example: export
*/

int	builtin_export(t_cmd *cmd, t_minishell *shell)
{
	// This function should implement the export command
	// For now, we will just print a placeholder message
	printf("EXPORT command executed\n");
	return (SUCCESS);
}
