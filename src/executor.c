/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:22:58 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/02 16:26:31 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* --BUILTINS--

echo (with -n option)

cd (change directory)

pwd (print working directory)

export (set environment variable)

unset (remove environment variable)

env (print environment variables)

exit (exit the shell)

--- --- --- --- --- --- --- --- ---

Everything else should be executed as an external program via 'execve'

--- --- --- --- --- --- --- --- ---
*/

#include "../include/minishell.h"
#include <unistd.h>
#include <signal.h>

int execute_command(t_cmd *cmd, t_minishell *shell) // Execution call function.
{
    if (!shell->envp || !cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
        return (FAILURE);

    if (is_builtin(cmd->args) == SUCCESS) // Parent process runs built-in commands
    {
        run_builtin(cmd, shell);
        return (SUCCESS);
    }
	else // Child process runs external commands with execve and fork
	{
		run_external(cmd, shell);
		return (SUCCESS);
	}
}
