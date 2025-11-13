/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 13:40:09 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/13 14:29:18 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_heredoc_files(t_redirect *redirects)
{
	t_redirect	*current;
	t_redirect	*last_heredoc;

	if (!redirects)
		return ;
	last_heredoc = NULL;
	current = redirects;
	while (current)
	{
		if (current->type == RED_HEREDOC)
			last_heredoc = current;
		current = current->next;
	}
	current = redirects;
	while (current)
	{
		if (current->type == RED_HEREDOC && current
			!= last_heredoc && current->filename)
			unlink(current->filename);
		current = current->next;
	}
}

char	*make_tmp_heredoc_filename(int i)
{
	char	*filename;
	char	*num;

	num = ft_itoa(i);
	if (!num)
		return (NULL);
	filename = ft_strjoin("/tmp/minishell_heredoc_", num);
	free(num);
	return (filename);
}
