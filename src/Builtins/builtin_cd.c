/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:01 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/18 18:59:44 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
    cd: cd with a relative or absolute path requires exactly 1 argument. 
	The path is that argument.
        Example: cd /home, cd ../documents
*/

int	builtin_cd(t_cmd *cmd, t_minishell *shell)
{
	// This function should implement the cd command
	// For now, we will just print a placeholder message
	printf("CD command executed\n");
	return (SUCCESS);
}
