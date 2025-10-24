/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:04:35 by jukerste          #+#    #+#             */
/*   Updated: 2025/10/24 17:38:50 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// count the amount of redirects found from input > < >>
int	count_redirects(t_redirect *list)
{
	int	len;

	len = 0;
	if (list)
	{
		while (list[len].filename != NULL)
			len++;
	}
	return (len);
}

// // add a new redirect to the list of redirects
t_redirect	*add_redirect(t_redirect *list, char *filename, t_redirect_type type, int expand)
{
	t_redirect	*new_redirect;
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
	new_redirect = malloc(sizeof(t_redirect));
	if (!new_redirect)
	{
		free(dup);
		return (NULL);
	}
	new_redirect->filename = dup;
	new_redirect->type = type;
	new_redirect->expand = expand;
	new_redirect->next = NULL;
	if (!list)
		return (new_redirect);
	current = list;
	while (current->next)
		current = current->next;
	current->next = new_redirect;
	return (list);
}
