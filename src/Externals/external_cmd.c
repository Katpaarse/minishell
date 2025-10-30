/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 16:24:09 by lavan-de          #+#    #+#             */
/*   Updated: 2025/10/27 12:16:10 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_path_cmd(t_cmd *cmd, t_minishell *shell)
{
    struct stat st;

    if (stat(cmd->args[0], &st) < 0)
    {
        print_error(shell, "No such file or directory");
        return (127);
    }
    if (S_ISDIR(st.st_mode))
    {
        print_error(shell, "Is a directory");
        return (126);
    }
    if (access(cmd->args[0], X_OK) != 0)
    {
        print_error(shell, "Permission denied");
        return (126);
    }

    // pid_t pid = fork();
    // if (pid < 0)
    // {
    //     perror("fork");
    //     return (FAILURE);
    // }
    // if (pid == 0)
    // {
    //     setup_child_signals();
    //     handle_redirects(cmd);
    //     execve(cmd->args[0], cmd->args, shell->envp);
    //     perror("execve failed");
    //     exit(126);
    // }
    // g_minishell_is_executing = pid;
    // shell->exit_code = wait_for_child(pid);
    // g_minishell_is_executing = 0;
	cmd_fork(cmd, shell, cmd->args[0]);
    return (shell->exit_code);
}
