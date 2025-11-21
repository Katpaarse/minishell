/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 15:46:17 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/21 12:18:29 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Temporarily disables SIGINT while preparing the heredoc */
static void	setup_heredoc_signals(struct sigaction *new_sa,
	struct sigaction *old_sa)
{
	ft_memset(new_sa, 0, sizeof(*new_sa));
	new_sa->sa_handler = SIG_IGN;
	sigemptyset(&new_sa->sa_mask);
	sigaction(SIGINT, new_sa, old_sa);
}

/* Checks the exit status of the heredoc child process */
static int	handle_heredoc_status(int status, t_minishell *shell,
	char *tmpfile)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		free(tmpfile);
		write(1, "\n", 1);
		shell->exit_code = 130;
		return (0);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		free(tmpfile);
		return (0);
	}
	return (1);
}

/* Looking if the heredoc delimiter should allow variable expansion */
static char	*get_heredoc_delimiter_info(char *raw_token, int *expand)
{
	size_t	len;
	char	*clean_delim;

	len = ft_strlen(raw_token);
	*expand = 1;
	if ((raw_token[0] == '\'' && raw_token[len - 1] == '\'')
		|| (raw_token[0] == '"' && raw_token[len - 1] == '"'))
		*expand = 0;
	clean_delim = remove_quotes(raw_token);
	return (clean_delim);
}

/* Creates and fills a temporary heredoc file */
char	*process_heredoc(char const *delimiter, int i, t_minishell *shell,
	int expand)
{
	pid_t				pid;
	int					status;
	char				*tmpfile;
	struct sigaction	old_sa;
	struct sigaction	new_sa;

	tmpfile = make_tmp_heredoc_filename(i);
	if (!tmpfile)
		return (NULL);
	setup_heredoc_signals(&new_sa, &old_sa);
	pid = fork();
	if (pid == 0)
	{
		sigaction(SIGINT, &old_sa, NULL);
		setup_heredoc_signal_handlers();
		handle_heredoc(delimiter, tmpfile, shell, expand);
		exit(0);
	}
	waitpid(pid, &status, 0);
	sigaction(SIGINT, &old_sa, NULL);
	if (!handle_heredoc_status(status, shell, tmpfile))
		return (NULL);
	return (tmpfile);
}

/* Processes a heredoc token during parsing */
int	handle_heredoc_token(t_cmd *current, char *raw_token, int heredoc_index,
	t_minishell *shell)
{
	int			expand;
	char		*tmpfile;
	char		*clean_delim;
	t_redirect	*new_list;

	if (!raw_token)
	{
		print_syntax_error(shell, NULL);
		return (-1);
	}
	clean_delim = get_heredoc_delimiter_info(raw_token, &expand);
	tmpfile = process_heredoc(clean_delim, heredoc_index, shell, expand);
	free(clean_delim);
	if (!tmpfile)
	{
		if (shell->exit_code == 130)
			return (-2);
		print_syntax_error(shell, NULL);
		return (-1);
	}
	new_list = add_redirect(current->redir, tmpfile, RED_HEREDOC);
	if (!new_list)
		return (-1);
	current->redir = new_list;
	return (0);
}
