/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:04:35 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/21 13:34:07 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Creates a new redirect node and appends it to the end of 
the redirect linked list */
t_redirect	*add_redirect(t_redirect *list, char *filename,
	t_redirect_type type)
{
	t_redirect	*new_redirect;
	t_redirect	*current;

	if (!filename)
		return (list);
	new_redirect = malloc(sizeof(t_redirect));
	if (!new_redirect)
	{
		free(filename);
		return (NULL);
	}
	new_redirect->filename = filename;
	new_redirect->type = type;
	new_redirect->next = NULL;
	if (!list)
		return (new_redirect);
	current = list;
	while (current->next)
		current = current->next;
	current->next = new_redirect;
	return (list);
}
