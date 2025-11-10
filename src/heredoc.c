/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 15:46:17 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/05 17:36:16 by jukerste         ###   ########.fr       */
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

void	handle_heredoc(char const *delimiter, char	*tmpfile, t_minishell *shell, int expand)
{
	char	*line;
	int		fd;
	char	*to_write;

	fd = open(tmpfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		perror("heredoc open");
		exit(1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("minishell: heredoc delimiter not specified\n");
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		to_write = line;
		if (expand)
		{
			to_write = expand_variables(line, shell);
			free(line);
		}
		write(fd, to_write, ft_strlen(to_write));
		write(fd, "\n", 1);
		if (expand)
			free(to_write);
	}
	close(fd);
}

char *process_heredoc(char const *delimiter, int i, t_minishell *shell, int expand)
{
	pid_t	pid;
	int		status;
	char	*tmpfile;
	struct sigaction old_sa;
	struct sigaction new_sa;

	tmpfile = make_tmp_heredoc_filename(i);
	if (!tmpfile)
		return (NULL);
	ft_memset(&new_sa, 0, sizeof(new_sa));
	new_sa.sa_handler = SIG_IGN;
	new_sa.sa_flags = 0;
	sigemptyset(&new_sa.sa_mask);
	sigaction(SIGINT, &new_sa, &old_sa);
	pid = fork();
	if (pid < 0)
	{
		free(tmpfile);
		sigaction(SIGINT, &old_sa, NULL);
		return (NULL);
	}
	if (pid == 0)
	{
		sigaction(SIGINT, &old_sa, NULL);
		setup_heredoc_signal_handlers();
		handle_heredoc(delimiter, tmpfile, shell, expand);
		exit(0);
	}
	waitpid(pid, &status, 0);
	sigaction(SIGINT, &old_sa, NULL);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		free(tmpfile);
		write(1, "\n", 1);
		shell->exit_code = 130;
		return (NULL);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
    {
        free(tmpfile);
        return (NULL);
    }
	return (tmpfile);
}

int	handle_heredoc_token(t_cmd *current, char *raw_token, int heredoc_index, t_minishell *shell)
{
	int		expand;
	size_t	len;
	char	*tmpfile;
	char	*clean_delim;

	if (!raw_token)
	{
		print_syntax_error(shell, NULL);
		return (-1);
	}
	len = ft_strlen(raw_token);
	expand = 1;
	if ((raw_token[0] == '\'' && raw_token[len - 1] == '\'') ||
		(raw_token[0] == '"' && raw_token[len - 1] == '"'))
		expand = 0;
	clean_delim = remove_quotes(raw_token);
	tmpfile = process_heredoc(clean_delim, heredoc_index, shell, expand);
	free(clean_delim);
	if (!tmpfile)
	{
		if (shell->exit_code == 130)
			return (-2);
		print_syntax_error(shell, NULL);
		return (-1);
	}
	{
		t_redirect	*new_list;

		new_list = add_redirect(current->redir, tmpfile, RED_HEREDOC, expand);
		if (!new_list)
			return (-1);
		current->redir = new_list;
	}
	return (0);
}
