/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 15:11:56 by jukerste          #+#    #+#             */
/*   Updated: 2025/03/08 15:59:08 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*read_and_stash(int fd, char *leftover, char *buffer)
{
	ssize_t	chars_read;
	char	*temp;

	chars_read = 1;
	while (chars_read > 0 && !ft_strchr(leftover, '\n'))
	{
		chars_read = read(fd, buffer, BUFFER_SIZE);
		if (chars_read == -1)
			return (free(leftover), NULL);
		if (chars_read == 0)
			break ;
		buffer[chars_read] = '\0';
		if (leftover == NULL)
			leftover = ft_strdup("");
		if (leftover == NULL)
			return (NULL);
		temp = leftover;
		leftover = ft_strjoin(temp, buffer);
		free(temp);
		if (leftover == NULL)
			return (NULL);
	}
	return (leftover);
}

char	*extract_line(char **line_buffer)
{
	char	*leftover;
	char	*temp_line;
	size_t	i;

	i = 0;
	if (*line_buffer == NULL)
		return (NULL);
	while ((*line_buffer)[i] != '\n' && (*line_buffer)[i] != '\0')
		i++;
	if ((*line_buffer)[i] == '\0')
		return (NULL);
	leftover = ft_substr(*line_buffer, i + 1, ft_strlen(*line_buffer) - i - 1);
	if (leftover == NULL)
		return (NULL);
	temp_line = ft_substr(*line_buffer, 0, i + 1);
	if (*leftover == '\0' || temp_line == NULL)
	{
		free(leftover);
		leftover = NULL;
	}
	free(*line_buffer);
	*line_buffer = temp_line;
	return (leftover);
}

char	*get_next_line(int fd)
{
	static char	*leftover;
	static char	buffer[BUFFER_SIZE];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = read_and_stash(fd, leftover, buffer);
	if (line == NULL)
	{
		leftover = NULL;
		return (NULL);
	}
	leftover = extract_line(&line);
	return (line);
}
