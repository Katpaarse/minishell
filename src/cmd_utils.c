/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 19:19:42 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/29 15:52:41 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Now about t_redirect initialization

Since t_redirect is a small struct, you’ll usually allocate it during parsing. For example, when you parse a > token:

t_redirect new_redir;
new_redir.filename = strdup("out.txt");
new_redir.type = RED_OUTPUT;

Then you push it into the cmd->redirects array (using malloc/realloc if necessary).

---

If you want a helper function for redirects

This can make life easier:

t_redirect	create_redirect(const char *filename, t_redirect_type type)
{
	t_redirect redir;

	redir.filename = strdup(filename);
	redir.type = type;
	return (redir);
}

Then in your parser:
cmd->redirects[i] = create_redirect("out.txt", RED_OUTPUT);
*/


// creates a new t_cmd node and initializes all its fields to default values
t_cmd	*cmd_into_new_node(void)
{
	t_cmd	*cmd;
	
	cmd = malloc(sizeof(t_cmd));
	if (cmd == NULL)
		return (NULL);
	cmd->args = NULL; // no argument yet and sets it to NULL
	cmd->redirects = NULL; // no redirection yet
	cmd->next = NULL; // no next command yet
	cmd->heredoc_delim = NULL; // set to NULL because not EOF yet
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
