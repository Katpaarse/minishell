/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:11 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/18 18:59:44 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
    export: export with no options requires 0 arguments.
        Example: export
*/

int	builtin_export(t_cmd *cmd, t_minishell *shell)
{
	getenv("PATH"); // Example usage of getenv, not necessary for export
	// This function should implement the export command
	// For now, we will just print a placeholder message
	printf("EXPORT command executed\n");
	return (SUCCESS);
}
