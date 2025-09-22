/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 19:19:42 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/22 15:37:16 by jukerste         ###   ########.fr       */
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
	if (!new_args)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[i] = new_arg;
	new_args[i + 1] = NULL;
	free(args);
	return (new_args);
}

void	free_redirects(t_redirect *list)
{
	int	i;
	
	if (!list)
		return ;
	i = 0;
	while (list[i].filename != NULL) // loop through each redirect
	{
		free(list[i].filename); // free strdup’d filename
		i++;
	}
	free(list); // free the array itself
}

void	free_args(char **args)
{
	int	i;
	
	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]); // free each argument string
		i++;
	}
	free(args); // free the array
}

void	free_cmds(t_cmd *cmd)
{
	t_cmd	*next;
	
	while (cmd != NULL)
	{
		next = cmd->next; // save the next node
		free_args(cmd->args); // free arguments
		free_redirects(cmd->redirects); // free redirects
		free(cmd); // free this node
		cmd = next; // move to next
	}
}
