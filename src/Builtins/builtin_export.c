/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:11 by lavan-de          #+#    #+#             */
/*   Updated: 2025/10/22 15:44:55 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_or_update_env(t_minishell *shell, char *var)
{
	int		i;
	int		len;
	int		size;
	char	**new_env;

	if (!shell || !var)
		return;
	// find length before '='
	len = 0;
	while (var[len] && var[len] != '=')
		len++;
	// search for existing var
	i = 0;
	while (shell->envp && shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], var, len) == 0
			&& (shell->envp[i][len] == '=' || shell->envp[i][len] == '\0'))
		{
			free(shell->envp[i]);
			shell->envp[i] = ft_strdup(var);
			return;
		}
		i++;
	}
	// count current size
	size = 0;
	while (shell->envp && shell->envp[size])
		size++;
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return;
	i = 0;
	while (i < size)
	{
		new_env[i] = shell->envp[i];
		i++;
	}
	new_env[size] = ft_strdup(var);
	new_env[size + 1] = NULL;
	free(shell->envp);
	shell->envp = new_env;
}

void	add_or_update_exp(t_minishell *shell, char *var)
{
	int		i;
	int		len;
	int		size;
	char	**new_exp;

	if (!shell || !var)
		return;
	len = 0;
	while (var[len] && var[len] != '=')
		len++;
	i = 0;
	while (shell->exp_list && shell->exp_list[i])
	{
		if (ft_strncmp(shell->exp_list[i], var, len) == 0
			&& (shell->exp_list[i][len] == '=' || shell->exp_list[i][len] == '\0'))
		{
			free(shell->exp_list[i]);
			shell->exp_list[i] = ft_strdup(var);
			return;
		}
		i++;
	}
	size = 0;
	while (shell->exp_list && shell->exp_list[size])
		size++;
	new_exp = malloc(sizeof(char *) * (size + 2));
	if (!new_exp)
		return;
	i = 0;
	while (i < size)
	{
		new_exp[i] = shell->exp_list[i];
		i++;
	}
	new_exp[size] = ft_strdup(var);
	new_exp[size + 1] = NULL;
	free(shell->exp_list);
	shell->exp_list = new_exp;
}

int	builtin_export(t_cmd *cmd, t_minishell *shell)
{
	int i;
	int j;
	int check;
	char *equal_sign;

	if (!cmd || !shell)
		return (FAILURE);

	i = 1;
	if (cmd->args[1] == NULL)
	{
		j = 0;
		while (shell->exp_list && shell->exp_list[j] != NULL)
		{
			write(1, "declare -x ", 11);
			if (ft_strchr(shell->exp_list[j], '=') != NULL)
			{
				equal_sign = ft_strchr(shell->exp_list[j], '=');
				write(1, shell->exp_list[j], equal_sign - shell->exp_list[j] + 1); // include '='
				write(1, "\"", 1);
				write(1, equal_sign + 1, ft_strlen(equal_sign + 1));
				write(1, "\"", 1);
			}
			else
				write(1, shell->exp_list[j], ft_strlen(shell->exp_list[j]));
			write(1, "\n", 1);
			j++;
		}
		return (SUCCESS);
	}
	i = 1;
	while (cmd->args[i])
	{
		j = 0;
		if ((cmd->args[i][0] == '_' || ft_isalpha(cmd->args[i][0])) == 1)
		{
			while (cmd->args[i][j] && cmd->args[i][j] != '=')
			{
				if (cmd->args[i][j] != '_' && ft_isalpha(cmd->args[i][j]) == 0
					&& ft_isdigit(cmd->args[i][j]) == 0)
				{
					// invalid variable name
					print_error(shell, "not a valid identifier\n");
					return (FAILURE);
				}
				j++;
			}			
			if (ft_strchr(cmd->args[i], '=') != NULL)
			{
				// handle 'space' values in var_value
				// Example: export VAR="value with spaces"
				// should be in " " quotes when printed with export
				// example: export VAR=" "
				if (cmd->args[i][j + 1] != '\0') // there is a value after '='
				{
					check = 0; // name=value
				}
				else // no value after '='
				{
					check = 1; // name=
				}
			}
			else // no '=' in argument, just a variable name
			{
				check = 2; // name
			}
		}
		else
		{
			// invalid variable name
			print_error(shell, "not a valid identifier");
			return (FAILURE);
		}
		if (check == 0 || check == 1) // NAME=VALUE or NAME= | 
		{
			add_or_update_exp(shell, cmd->args[i]);
			add_or_update_env(shell, cmd->args[i]);
		}
		else if (check == 2) // NAME
			add_or_update_exp(shell, cmd->args[i]);
		i++;
	}
	return (SUCCESS);
}
