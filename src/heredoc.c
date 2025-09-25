/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 15:46:17 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/25 19:28:33 by jukerste         ###   ########.fr       */
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
		if (ft_strcmp(line, delimiter) == 0) // found heredoc char
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

char *process_heredoc(char const *delimiter, int i)
{
	pid_t	pid;
	int 	status;
	char	*tmpfile;
	
	tmpfile = NULL;
	pid = fork();
	if (pid < 0)
		return (NULL);
	if (pid == 0)
	{
		tmpfile = handle_heredoc(delimiter, i);
		exit(0);
	}
	waitpid(pid, &status, 0);
	tmpfile = make_tmp_heredoc_filename(i);
	return (tmpfile);
}
