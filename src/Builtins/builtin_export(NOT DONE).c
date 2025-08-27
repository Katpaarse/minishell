/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export(NOT DONE).c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:11 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/21 19:36:07 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
export explanation:
The main benefit of having a variable in the export list is that it becomes available to child processes your shell spawns.

In Unix-like systems, each process has its own environment.

By default, a variable you define in the shell (e.g., VAR=value) is local to the shell process.

When you export VAR, it’s added to the environment array. Then, if you run a command like ./program or ls, the child process inherits that environment, and can read VAR.

Example:
$ MYVAR=hello
$ ./print_var_program  # this program prints MYVAR
# Nothing printed, because MYVAR isn't exported

$ export MYVAR
$ ./print_var_program
hello  # now the child process sees it
*/

#include "minishell.h"

// ft_strncmp
// ft_strlen
// malloc/free
// write
/*
    export: export with no options requires 0 arguments.
        Example: export
*/

int	builtin_export(t_cmd *cmd, t_minishell *shell)
{
	int i;
	char *var_name;
	char *var_value;

	if (!cmd || !shell)
		return (FAILURE);

	i = 0;
	if (cmd->args[1] == NULL)
	{
		// print all env variables in shell->envp
		// each variable should be prefixed with "declare -x "
		// Example: declare -x PATH="/usr/bin:/bin"
		int j = 0;
		while (shell->envp[j])
		{
			write(1, "declare -x ", 11);
			write(1, shell->envp[j], ft_strlen(shell->envp[j]));
			write(1, "\n", 1);
			j++;
		}
		return (SUCCESS);
	}
	else
		while (cmd->args[++i] && (cmd->args[i][0] == '_' || ft_isalpha(cmd->args[i][0]) == 0))
		{
			if (ft_strchr(cmd->args[i], '=') == NULL)
			{
				write(2, "export: not a valid identifier\n", 31);
				return (FAILURE);
			}
			else
			{
				var_name = ft_strjoin_and_free(ft_strndup(cmd->args[i], ft_strchr(cmd->args[i], '=') - cmd->args[i]), "=");
				var_value = ft_strdup(ft_strchr(cmd->args[i], '=') + 1);

				if (setenv(var_name, var_value, 1) == -1)
				{
					free(var_name);
					free(var_value);
					return (FAILURE);
				}
				free(var_name);
				free(var_value);
			}
		}
	
	return (SUCCESS);
}
