/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:22:58 by lavan-de          #+#    #+#             */
/*   Updated: 2025/10/24 17:11:38 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_minishell *shell)
{
	if (!shell || !shell->cmds)
		return (FAILURE);
	if (!shell->cmds->next && is_builtin(shell->cmds) == SUCCESS)
	{
		shell->exit_code = run_builtin(shell->cmds, shell);
		return (SUCCESS);
	}
	if (shell->cmds->next != NULL)
	{
		execute_pipeline(shell, shell->cmds);
		return (SUCCESS);
	}
	shell->exit_code = run_external(shell->cmds, shell);
	return (SUCCESS);
}
