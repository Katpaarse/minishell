/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 15:46:17 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/26 17:48:51 by jukerste         ###   ########.fr       */
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
	filename = ft_strjoin("/tmp/minishell_heredoc_", num);
	free(num);
	return (filename);
}

// static char	*read_line_simple(void)
// {
// 	char	*line;
// 	char	buffer[1024];
// 	int		i;
// 	int		c;
	
// 	write(1, "> ", 2);
// 	i = 0;
// 	while (i < 1023) // buffer can hold 1024 characters
// 	{
// 		if (read(0, &c, 1) < 0)
// 			return (NULL);
// 		if (c == '\n')
// 			break ;
// 		buffer[i] = c;
// 		i++;
// 	}
// 	buffer[i] = '\0';
// 	line = ft_strdup(buffer);
// 	return (line);
// }

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
	setup_heredoc_signal_handlers();
	while (1)
	{
		line = readline("> ");
		if (!line) // user pressed ctrl + D
		{
			printf("minishell: heredoc delimiter not specified\n");
			break;
		}
		if (ft_strcmp(line, delimiter) == 0) // found heredoc char
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	setup_signal_handlers();
	close(fd);
	return (tmpfile);
}

// deze functie wordt later gebruikt voor splitten van de handle_heredoc functie. So keep it for now.. 
char *process_heredoc(char const *delimiter, int i)
{
	return (handle_heredoc(delimiter, i));
}
