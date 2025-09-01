/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:22:58 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/13 18:55:02 by lavan-de         ###   ########.fr       */
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

int execute_command(char **argv, char **envp, t_minishell *shell, t_cmd *cmd)
{
    if (!envp || !argv || !argv[0] || argv[0][0] == '\0')
        return (FAILURE);

    if (is_builtin(argv) == SUCCESS) // Parent process runs built-in commands
    {
        run_builtin(argv, shell, cmd);
        return (SUCCESS);
    }
	else // Child process runs external commands with execve and fork
	{
		run_external(argv, shell->envp);
		return (SUCCESS);
	}
}


