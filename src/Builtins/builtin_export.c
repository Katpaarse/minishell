/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:11 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/02 16:40:11 by jukerste         ###   ########.fr       */
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

// int	builtin_export(t_cmd *cmd, t_minishell *shell)
// {
// 	int i;
// 	int j;
// 	int check;
// 	char *var_name;
// 	char *var_value;

// 	if (!cmd || !shell)
// 		return (FAILURE);

// 	i = 1;
// 	if (cmd->args[1] == NULL)
// 	{
// 		// print all env variables in shell->envp
// 		// each variable should be prefixed with "declare -x "
// 		// Example: declare -x PATH="/usr/bin:/bin"
// 		j = 0;
// 		while (shell->envp[j])
// 		{
// 			write(1, "declare -x ", 11);
// 			write(1, shell->envp[j], ft_strlen(shell->envp[j]));
// 			write(1, "\n", 1);
// 			j++;
// 		}
// 		return (SUCCESS);
// 	}
// 	else
// 	{
// 		while (cmd->args[i])
// 		{
// 			j = 0;
// 			if (cmd->args[i][0] == '_' || ft_isalpha(cmd->args[i][0]) == 1)
// 			{
// 				if (ft_strchr(cmd->args[i], '=') != NULL)
// 				{
// 					// handle 'space' values in var_value
// 					// Example: export VAR="value with spaces"
// 					// should be in " " quotes when printed with export
// 					// example: export VAR=" "
// 					while (cmd->args[i][j] && cmd->args[i][j] != '=')
// 						j++;
// 					if (cmd->args[i][j + 1] != '\0') // there is a value after '='
// 					{
// 						var_name = ft_strndup(cmd->args[i], j + 1); // include '='
// 						var_value = ft_strdup(&cmd->args[i][j + 1]);
// 						check = 0;
// 					}
// 					else // no value after '='
// 					{
// 						var_name = ft_strndup(cmd->args[i], j + 1); // include '='
// 						var_value = ft_strdup("");
// 						check = 1;
// 					}
// 				}
// 				else // no '=' in argument, just a variable name
// 				{
// 					var_name = ft_strdup(cmd->args[i]);
// 					var_value = NULL;
// 					check = 2;
// 				}
// 			}
// 			i++;
// 		}
// 	}

// 	if (check == 0)
// 	{
// 		// add or update var_name with var_value in shell->envp
// 		// also add to shell->exp_list if not already present
// 	}
// 	else if (check == 1)
// 	{
// 		// add or update var_name with empty value in shell->envp
// 		// also add to shell->exp_list if not already present
// 	}
// 	else if (check == 2)
// 	{
// 		// just add var_name to shell->exp_list if not already present
// 	}

// 	return (SUCCESS);
// }

// delete later. This is for testing
int builtin_export(char **args, char ***env)
{
    (void)args;
    (void)env;
    // Temporary stub — will implement later
    return 0;
}
