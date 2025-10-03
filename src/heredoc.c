/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 15:46:17 by jukerste          #+#    #+#             */
/*   Updated: 2025/10/02 15:24:01 by jukerste         ###   ########.fr       */
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
	while (current) // find the last heredoc
	{
		if (current->type == RED_HEREDOC)
			last_heredoc = current;
		current = current->next;
	}
	current = redirects;
	while (current) // delete all heredoc tmp files except the last one
	{
		if (current->type == RED_HEREDOC && current != last_heredoc && current->filename)
			unlink(current->filename);
		current = current->next;
	}
}

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

char	*handle_heredoc(char const *delimiter, char	*tmpfile)
{
	char	*line;
	int		fd;

	fd = open(tmpfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		perror("heredoc open");
		return (NULL);
	}
	// setup_heredoc_signal_handlers();
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
		// printf("GAAT DE LOOP DOOR?\n");
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (tmpfile);
}

// deze functie wordt later gebruikt voor splitten van de handle_heredoc functie. So keep it for now.. 
char *process_heredoc(char const *delimiter, int i)
{
	pid_t	pid;
	int		status;
	char	*tmpfile;
	struct sigaction old_sa;
	struct sigaction new_sa;


	tmpfile = make_tmp_heredoc_filename(i);
	if (!tmpfile)
		return (NULL);
	
	pid = fork();
	if (pid < 0)
	{
		free(tmpfile);
		return (NULL);
	}
	if (pid == 0) // child process. Write to the file
	{
		setup_heredoc_signal_handlers();
		handle_heredoc(delimiter, tmpfile); // pass the filename to write to
		exit(0);
	}

	ft_memset(&new_sa, 0, sizeof(new_sa));
	new_sa.sa_handler = SIG_IGN;
	new_sa.sa_flags = 0;
	sigemptyset(&new_sa.sa_mask);
	sigaction(SIGINT, &new_sa, &old_sa);

	waitpid(pid, &status, 0);
	
	sigaction(SIGINT, &old_sa, NULL);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		free(tmpfile);
		write(1, "\n", 1);
		return (NULL);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
    {
        free(tmpfile);
        return (NULL);
    }
	return (tmpfile);
}
