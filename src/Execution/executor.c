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

/* Main execution hub: delegates to builtins or external commands. Checks for
single builtins (run in parent) or pipelines/externals (run in child) */
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

/* Handles execution of external (non-builtin) commands. Resolves command path
(absolute/relative/PATH) and handles execution errors like 127/126 */
int	run_external(t_cmd *cmd, t_minishell *shell)
{
	char	*cmd_path;

	if (ft_strchr(cmd->args[0], '/'))
		return (execute_path_cmd(cmd, shell));
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
	cmd_fork(cmd, shell, cmd_path);
	free(cmd_path);
	return (shell->exit_code);
}

/* Routes execution to parent or child builtins based on type. Distinguishes
between builtins that modify shell state (parent) vs those that don't (child) */
int	run_builtin(t_cmd *cmd, t_minishell *shell)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (FAILURE);
	if (is_parent_builtin(cmd) == SUCCESS)
		return (run_parent_builtin(cmd, shell));
	return (run_child_builtin(cmd, shell));
}

/* Executes builtins that must run in the parent process. Manages temporary
redirects for the parent process to avoid permanent changes to FDs. */
int	run_parent_builtin(t_cmd *cmd, t_minishell *shell)
{
	int	saved_in;
	int	saved_out;
	int	result;

	if (setup_parent_redirects(cmd, &saved_in, &saved_out) == FAILURE)
		return (FAILURE);
	result = execute_builtin(cmd, shell);
	shell->exit_code = result;
	return (restore_parent_redirects(saved_in, saved_out, result));
}

/* Executes builtins that can run in a child process. Forks a new process for
redirects if needed, otherwise runs directly. */
int	run_child_builtin(t_cmd *cmd, t_minishell *shell)
{
	int	result;

	if (cmd->redir)
		result = child_redirects(cmd, shell, 0);
	else
		result = execute_builtin(cmd, shell);
	shell->exit_code = result;
	return (result);
}
