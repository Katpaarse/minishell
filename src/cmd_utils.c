/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 19:19:42 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/15 19:51:46 by jukerste         ###   ########.fr       */
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
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = FALSE;
	cmd->next = NULL;
	return (cmd);
}

char	**add_argument(char **args, char *arg)
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
		new_args[i] = arg;
		new_args [i + 1] = NULL;
		free(args);
		return (new_args);
	}
}
