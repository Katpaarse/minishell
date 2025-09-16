/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:04:35 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/01 16:45:15 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// count the amount of redirects found from input
int	count_redirects(t_redirect *list)
{
	int	len;

	len = 0;
	if (list) // if list == NULL then there is no redirects found in list
	{
		while (list[len].filename != NULL)
			len++;
	}
	return (len);
}
// add a new redirect to the list of redirects
t_redirect	*add_redirect(t_redirect *list, char *filename, t_redirect_type type)
{
	int			i;
	int			len;
	t_redirect	*new_list;

	len = count_redirects(list);
	new_list = malloc(sizeof(t_redirect) * (len + 2));
	if (new_list == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_list[i].filename = list[i].filename;
		new_list[i].type = list[i].type;
		i++;
	}
	new_list[len].filename = filename;
	new_list[len].type = type;
	new_list[len + 1].filename = NULL;
	new_list[len + 1].type = RED_NONE;
	free(list);
	return (new_list);
}

int	is_redirect(t_cmd *cmd, t_minishell *shell)
{
	int fd;
	int	i;

	if (!cmd || !shell || !cmd->redirects)
		return (SUCCESS);

	i = 0;
	while (cmd->redirects && cmd->redirects[i].type != RED_NONE)
	{
		if (cmd->redirects[i].type == RED_OUTPUT)
			fd = open(cmd->redirects[i].filename, O_WRONLY | O_CREAT | O_TRUNC, 0666); 	// '>' TRUNCATE mode
		else if (cmd->redirects[i].type == RED_APPEND)
			fd = open(cmd->redirects[i].filename, O_WRONLY | O_CREAT | O_APPEND, 0666); // '>>' APPEND mode
		else if (cmd->redirects[i].type == RED_INPUT)
			fd = open(cmd->redirects[i].filename, O_RDONLY);							// '<' INFILE mode
		else if (cmd->redirects[i].type == RED_HEREDOC)
		{																				// '<<' HEREDOC 'EOF' mode
			// implement heredoc handling
			i++;
			continue ;
		}
		else
		{
			i++;
			continue ;
		}

		if (fd < 0)
		{
			// error handling here
			return (FAILURE);
		}

		// Redirect stdout or stdin depending on type
		if (cmd->redirects[i].type == RED_OUTPUT || cmd->redirects[i].type == RED_APPEND)
		{
			if (dup2(fd, 1) < 0)
			{
				// error handling here
				close(fd);
				return (FAILURE);
			}
		}
		else if (cmd->redirects[i].type == RED_INPUT || cmd->redirects[i].type == RED_HEREDOC)
		{
			if (dup2(fd, 0) < 0)
			{
				// error handling here
				close(fd);
				return (FAILURE);
			}
		}

		close(fd);
		i++;
	}

	return (SUCCESS);
}
