/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:04:35 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/30 13:21:51 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// count the amount of redirects found from input > < >>
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

// // add a new redirect to the list of redirects
t_redirect	*add_redirect(t_redirect *list, char *filename, t_redirect_type type)
{
	t_redirect	*new_redirection;
	t_redirect	*current;
	char		*dup;

	if (!filename)
		return (list);

	dup = ft_strdup(filename);
	if (!dup)
	{
		free(filename);
		return NULL;
	}
	free(filename);
	new_redirection = malloc(sizeof(t_redirect));
	if (!new_redirection)
	{
		free(dup);
		return (NULL);
	}
	new_redirection->filename = dup;
	new_redirection->type = type;
	new_redirection->next = NULL;
	if (!list)
		return (new_redirection);
	current = list;
	while (current->next)
		current = current->next;
	current->next = new_redirection;
	return (list);
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
			fd = open(cmd->redirects[i].filename, O_RDONLY); // heredoc is already handled in parsing
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
