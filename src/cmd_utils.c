/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 19:19:42 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/17 15:27:59 by jukerste         ###   ########.fr       */
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
	cmd->infile = NULL; // no input redirection yet
	cmd->outfile = NULL; // no output redirection yet
	cmd->append = FALSE; // not appending by default
	cmd->next = NULL; // no next command yet
	return (cmd);
}

// add a new argument to a commands argument array (args) in t_cmd struct
char	**add_argument(char **args, char *new_arg)
{
	int		i;
	int		count;
	char	**new_args;

	count = 0;
	while (args && args[count])
		count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (new_args == NULL)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[i] = new_arg;
	new_args [i + 1] = NULL;
	free(args);
	return (new_args);
}
