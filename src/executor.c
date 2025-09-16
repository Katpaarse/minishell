/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:22:58 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/15 16:58:47 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_minishell *shell)
{
	if (!shell->envp || !shell->cmds->args || !shell->cmds->args[0] || shell->cmds->args[0][0] == '\0')
		return (FAILURE);
	if (is_builtin(shell->cmds->args) == SUCCESS && shell->cmds->next == NULL)
	{
		run_builtin(shell->cmds, shell);
		return (SUCCESS);
	}
	else if (shell->cmds->next != NULL)
	{
		execute_pipeline(shell, shell->cmds);
		return (SUCCESS);
	}
	else
	{
		run_external(shell->cmds, shell);
		return (SUCCESS);
	}
}
