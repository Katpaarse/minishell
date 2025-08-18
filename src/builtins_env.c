/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:17:21 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/14 18:17:22 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
    echo: echo with the -n option requires 0 or more arguments 
	(including the -n option itself and any text to be printed).
        Example: echo hello, echo -n hello world

    cd: cd with a relative or absolute path requires exactly 1 argument. 
	The path is that argument.
        Example: cd /home, cd ../documents

    pwd: pwd with no options requires 0 arguments.
        Example: pwd

    export: export with no options requires 0 arguments.
        Example: export

    unset: unset with no options requires 0 arguments.
        Example: unset

    env: env with no options or arguments requires 0 arguments.
        Example: env

    exit: exit with no options requires 0 arguments.
        Example: exit
*/


int	builtin_cd(t_cmd *cmd, t_minishell *shell)
{
	// This function should implement the cd command
	// For now, we will just print a placeholder message
	printf("CD command executed\n");
	return (SUCCESS);
}

int	builtin_env(t_minishell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i] != NULL)
	{
		printf("%s\n", shell->envp[i]);
		i++;
	}
	return (SUCCESS);
}

int	builtin_export(t_cmd *cmd, t_minishell *shell)
{
	// This function should implement the export command
	// For now, we will just print a placeholder message
	printf("EXPORT command executed\n");
	return (SUCCESS);
}

int	builtin_unset(t_cmd *cmd, t_minishell *shell)
{
	int i;
	(void)cmd;

	i = 1;
	while (cmd->args[i] != NULL)
	{
		if (cmd->args[i][0] == '\0')
		{
			fprintf(stderr, "unset: `%s': not a valid identifier\n", cmd->args[i]);
			return (FAILURE);
		}

		i++;
	}
	return (SUCCESS);
}

void	copy_envp(t_minishell *shell, char **envp)
{
	int i;

	i = 0;
	if (!envp || !shell)
		return;

	// Count the number of environment variables
	while (envp[i] != NULL)
		i++;
	
	shell->envp = malloc(sizeof(char *) * (i + 1));
	if (!shell->envp)
	{
		perror("envp malloc failed");
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (envp[i] != NULL)
	{
		shell->envp[i] = ft_strdup(envp[i]);
		if (!shell->envp[i])
		{
			perror("ft_strdup failed");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	shell->envp[i] = NULL; // Null-terminate the array
}
