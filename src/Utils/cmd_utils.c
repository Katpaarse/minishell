/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 19:19:42 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/20 17:02:43 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*cmd_into_new_node(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (cmd == NULL)
		return (NULL);
	cmd->args = NULL;
	cmd->redir = NULL;
	cmd->next = NULL;
	return (cmd);
}

/* Append a new argument to the command's args array (reallocates the array) */
char	**add_argument(char **args, char *new_arg)
{
	int		count;
	int		i;
	char	**new_args;

	if (!new_arg)
		return (args);
	count = 0;
	while (args && args[count])
		count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
	{
		free(new_arg);
		return (NULL);
	}
	i = 0;
	while (i < count)
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[count] = new_arg;
	new_args[count + 1] = NULL;
	free(args);
	return (new_args);
}
