/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:22:58 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/30 13:29:11 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_command(t_minishell *shell)
{
    if (!shell || !shell->cmds)
        return (FAILURE);
    if (!shell->cmds->args || !shell->cmds->args[0] || shell->cmds->args[0][0] == '\0')
        return (FAILURE);
    // Single parent-only builtin (like cd, export, unset)
    if (!shell->cmds->next && is_parent_builtin(shell->cmds))
    {
        shell->exit_code = run_builtin(shell->cmds, shell);
        return (SUCCESS);
    }
    // Pipeline
    if (shell->cmds->next != NULL)
    {
        execute_pipeline(shell, shell->cmds);
        return (SUCCESS);
    }
    // Single external command with redirections
    shell->exit_code = run_external(shell->cmds, shell);
    run_external(shell->cmds, shell);
    return (SUCCESS);
}
