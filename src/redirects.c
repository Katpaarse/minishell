/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:04:35 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/16 15:26:37 by jukerste         ###   ########.fr       */
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
t_redirect	*add_redirect(t_redirect *list, char *filename, t_redirect_type type)
{
	int			i;
	int			len;
	t_redirect	*new_list;

	len = count_redirects(list);
	new_list = malloc(sizeof(t_redirect) * (len + 2)); // + 1 for new redirect added to list and + 1 for sentinal null terminator for list
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
