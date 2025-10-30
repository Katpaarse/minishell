/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 19:19:42 by jukerste          #+#    #+#             */
/*   Updated: 2025/10/27 14:45:33 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// creates a new t_cmd node and initializes all its fields to default values
t_cmd	*cmd_into_new_node(void)
{
	t_cmd	*cmd;
	
	cmd = malloc(sizeof(t_cmd));
	if (cmd == NULL)
		return (NULL);
	cmd->args = NULL; // no argument yet and sets it to NULL
	cmd->redirects = NULL; // no redirects yet
	cmd->next = NULL; // no next command yet
	return (cmd);
}

char	**add_argument(char **args, char *new_arg)
{
	int		count;
	int		i;
	char	**new_args;
	char	*dup;

	if (!new_arg)
		return args;
	dup = ft_strdup(new_arg);
	if (!dup)
	{
		free(new_arg);
		return NULL;
	}
	free(new_arg);

	count = 0;
	while (args && args[count])
		count++;

	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
	{
		free(dup);
		return NULL;
	}

	i = 0;
	while (i < count)
	{
		new_args[i] = args[i]; // reuse old pointers
		i++;
	}
	new_args[count] = dup;
	new_args[count + 1] = NULL;

	free(args); // free old array (not the strings)
	return new_args;
}
