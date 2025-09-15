/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:22:58 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/02 16:28:14 by jukerste         ###   ########.fr       */
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

/*
❌ What’s missing / needs improvement:

1. Pipelines (biggest missing piece right now)

	- Currently you only handle one command at a time (shell->cmds).

	- You need to implement cmd1 | cmd2 | cmd3 handling:

		* Create pipes (pipe()).

		* Fork each stage.

		* Connect stdout of one command to stdin of the next.

		* Handle builtin-in-pipeline → run it in the child (effects won’t persist).

2. Parent builtin execution rules

	- Right now, your execute_command just checks if it’s a builtin and always runs it in the parent.

	- You need to update it so:

		* No pipeline: run parent builtins in parent, others fork if needed.

		* Pipeline: fork every stage, even parent builtins.

3. Heredoc (<<) support

	- Needs implementation in is_redirect.

	- Typically:

		* Create a temp file or pipe.

		* Read input until delimiter.

		* Feed that into stdin of the command.

4. Signal handling

	- You don’t yet handle Ctrl-C, Ctrl-\, Ctrl-D.

	- In parent: ignore SIGQUIT, handle SIGINT (print newline + reset prompt).

	- In child: restore default behavior.

5. Exit status handling

	- Right now you return wait_for_child(pid) → but that gives you raw status.

	- You should translate with WIFEXITED / WEXITSTATUS to store $? in shell->exit_status.

6. Memory cleanup

	- You return full_path from find_relative_path but don’t always free the paths array.

	- Same in other utils.

7. Edge cases

	- Empty commands.

	- Commands not found → print command not found.

	- Exec with no permissions.

	- Builtin error messages (match bash closely).
*/

#include "minishell.h"

int execute_command(t_minishell *shell) // Execution call function.
{
	// is_parent_builtin
	// is_redirect
	// execute_pipeline

    if (!shell->envp || !shell->cmds->args || !shell->cmds->args[0] || shell->cmds->args[0][0] == '\0')
        return (FAILURE);

    if (is_builtin(shell->cmds->args) == SUCCESS) // Parent process runs built-in commands
    {
        run_builtin(shell->cmds, shell);
        return (SUCCESS);
    }
	else // Child process runs external commands with execve and fork
	{
		run_external(shell->cmds->args, shell->envp);
		return (SUCCESS);
	}
}
