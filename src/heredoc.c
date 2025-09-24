/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 15:46:17 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/24 18:41:24 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*make_tmp_heredoc_filename(int	i)
{
	char	*filename;
	char	*num;

	num = ft_itoa(i);
	if (!num)
		return (NULL);
	filename = ft_strjoin_and_free(ft_strdup("/tmp/minishell_heredoc_"), num);
	free(num);
	return (filename);
}
char	*handle_heredoc(char const *delimiter, int i)
{
	char	*line;
	char	*tmpfile;
	int		fd;

	tmpfile = make_tmp_heredoc_filename(i);
	if (!tmpfile)
		return (NULL);
	fd = open(tmpfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		perror("heredoc open");
		free(tmpfile);
		return (NULL);
	}
	while (1)
	{
		line = readline("> ");
		if (!line) // user pressed ctrl + D
			break;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0) // found heredoc char
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (tmpfile);
}
