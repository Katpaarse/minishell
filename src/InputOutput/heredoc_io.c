/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_io.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:27:51 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/21 12:27:57 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Opens (or creates) the temporary heredoc file for writing */
static int	open_heredoc_file(char *tmpfile)
{
	int	fd;

	fd = open(tmpfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		perror("heredoc open");
		exit(1);
	}
	return (fd);
}

static int	is_delimiter(char *line, char const *delimiter)
{
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

/* Writes a single line into the heredoc file.
If expansion is enabled, expands variables before writing */
static void	write_heredoc_line(int fd, char *line, t_minishell *shell,
	int expand)
{
	char	*to_write;

	to_write = line;
	if (expand)
	{
		to_write = variable_expansion(line, shell);
		free(line);
	}
	write(fd, to_write, ft_strlen(to_write));
	write(fd, "\n", 1);
	if (expand)
		free(to_write);
}

/* Reads lines from the user until the delimiter is found. Writes each line 
into the temporary heredoc file, optionally performing variable expansion.
Stops when the delimiter is matched or EOF is reached */
void	handle_heredoc(const char *delimiter, char *tmpfile, t_minishell *shell,
	int expand)
{
	char	*line;
	int		fd;

	fd = open_heredoc_file(tmpfile);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("minishell: heredoc delimiter not specified\n");
			break ;
		}
		if (is_delimiter(line, delimiter))
			break ;
		write_heredoc_line(fd, line, shell, expand);
	}
	close(fd);
}
