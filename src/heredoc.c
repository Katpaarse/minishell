/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 15:46:17 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/23 14:26:43 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*make_temp_heredoc_filename(int	i)
{
	char	*filename;
	char	*num;

	num = ft_itoa(i);
	if (!num)
		return (NULL);
	filename = ft_strjoin_and_free(ft_strdup("/tmp/minishell_heredoc_"), i);
	free(num);
	return (filename);
}
static int	handle_heredoc(char *delimiter, int i, t_minishell *shell)
{
	char	*line;
	char	*filename;
	int		fd;
	
	filename = make_temp_heredoc_filename(i);
	if (!i)
		return (-1);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	
}
