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

static int execute_path_cmd(t_cmd *cmd, t_minishell *shell)
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

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return (FAILURE);
    }
    if (pid == 0)
    {
        setup_child_signals();
        handle_redirects(cmd);
        execve(cmd->args[0], cmd->args, shell->envp);
        perror("execve failed");
        exit(126);
    }
    g_minishell_is_executing = pid;
    shell->exit_code = wait_for_child(pid);
    g_minishell_is_executing = 0;
    return (shell->exit_code);
}

int run_external(t_cmd *cmd, t_minishell *shell)
{
    pid_t	pid;
    char 	*cmd_path;

    if (!cmd || !cmd->args || !cmd->args[0])
    {
        print_error(shell, "Invalid command");
        return (127);
    }

    // 🔹 Detect commands containing '/'
    if (ft_strchr(cmd->args[0], '/'))
        return execute_path_cmd(cmd, shell);

    // 🔹 Otherwise, search in PATH
    cmd_path = find_cmd_path(cmd->args, shell->envp);
    if (!cmd_path)
    {
        print_error(shell, "command not found");
        return (127);
    }
    if (access(cmd_path, X_OK) != 0)
    {
        print_error(shell, "Permission denied");
        free(cmd_path);
        return (126);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        free(cmd_path);
        return (FAILURE);
    }
    if (pid == 0)
    {
        setup_child_signals();
        handle_redirects(cmd);
        execve(cmd_path, cmd->args, shell->envp);
        perror("execve failed");
        exit(126);
    }
    free(cmd_path);
    g_minishell_is_executing = pid;
    shell->exit_code = wait_for_child(pid);
    g_minishell_is_executing = 0;
    return (shell->exit_code);
}
