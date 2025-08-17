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

int	builtin_cd(t_cmd *cmd, t_minishell *shell)
{
	// This function should implement the cd command
	// For now, we will just print a placeholder message
	printf("CD command executed\n");
	return (SUCCESS);
}

int	builtin_env(t_cmd *cmd, t_minishell *shell)
{
	// This function should implement the env command
	// For now, we will just print a placeholder message
	printf("ENV command executed\n");
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
	// This function should implement the unset command
	// For now, we will just print a placeholder message
	printf("UNSET command executed\n");
	return (SUCCESS);
}

void	copy_envp(t_minishell *shell, char **envp)
{
	int i;

	if (!envp || !shell)
		return;

	// Count the number of environment variables
	for (i = 0; envp[i]; i++);
	
	// Allocate memory for the envp in minishell structure
	shell->envp = malloc(sizeof(char *) * (i + 1));
	if (!shell->envp)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}

	// Copy the environment variables
	for (i = 0; envp[i]; i++)
	{
		shell->envp[i] = ft_strdup(envp[i]);
		if (!shell->envp[i])
		{
			perror("ft_strdup failed");
			exit(EXIT_FAILURE);
		}
	}
	shell->envp[i] = NULL; // Null-terminate the array
}
