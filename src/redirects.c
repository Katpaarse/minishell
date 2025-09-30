/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:04:35 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/30 18:12:46 by jukerste         ###   ########.fr       */
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
