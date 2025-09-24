/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:22:58 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/24 19:55:59 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void setup_redirections(t_cmd *cmd)
{
    int fd;
    t_redirect *redirection;

    redirection = cmd->redirects;
    while (redirection)
    {
        if (redirection->filename)
        {
            if (redirection->type == RED_HEREDOC || redirection->type == RED_INPUT)
                fd = open(redirection->filename, O_RDONLY);
            else if (redirection->type == RED_OUTPUT)
                fd = open(redirection->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else if (redirection->type == RED_APPEND)
                fd = open(redirection->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            else
            {
                redirection = redirection->next;
                continue;
            }

            if (fd < 0)
                perror(redirection->filename);
            else
            {
                dup2(fd, (redirection->type == RED_OUTPUT || redirection->type == RED_APPEND) ? 1 : 0);
                close(fd);
            }
        }
        redirection = redirection->next;
    }
}

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
    setup_redirections(shell->cmds);
    run_external(shell->cmds, shell);
    return (SUCCESS);
}
