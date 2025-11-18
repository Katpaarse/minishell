/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:04:35 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/18 12:39:01 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_redirect	*add_redirect(t_redirect *list, char *filename, t_redirect_type type)
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
