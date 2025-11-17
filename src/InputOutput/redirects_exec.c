/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 16:46:54 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/17 17:31:23 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redirect	*find_last_heredoc(t_redirect *redirect)
{
	t_redirect	*last;

	last = NULL;
	while (redirect)
	{
		if (redirect->type == RED_HEREDOC)
			last = redirect;
		redirect = redirect->next;
	}
	return (last);
}

static int	open_redirect_file(t_redirect *redirect, t_redirect *last,
	int *target_fd)
{
	int	fd;

	if (redirect->type == RED_HEREDOC || redirect->type == RED_INPUT)
	{
		if (redirect->type == RED_HEREDOC && redirect != last)
			return (-2);
		fd = open(redirect->filename, O_RDONLY);
		*target_fd = STDIN_FILENO;
	}
	else if (redirect->type == RED_OUTPUT)
	{
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		*target_fd = STDOUT_FILENO;
	}
	else if (redirect->type == RED_APPEND)
	{
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		*target_fd = STDOUT_FILENO;
	}
	else
		return (-2);
	return (fd);
}

static int	handle_redirect_error(t_redirect *redirect)
{
	if (errno == EACCES || errno == EISDIR)
		print_error_filename(redirect->filename, "Permission denied");
	else
		print_error_filename(redirect->filename, "No such file or directory");
	return (FAILURE);
}

static int	process_redirect(t_redirect *redirect, t_redirect *last)
{
	int	fd;
	int	target_fd;

	fd = open_redirect_file(redirect, last, &target_fd);
	if (fd == -2)
		return (SUCCESS);
	if (fd < 0)
		return (handle_redirect_error(redirect));
	if (dup2(fd, target_fd) == -1)
	{
		close(fd);
		print_error_filename(redirect->filename, "Redirection error");
		return (FAILURE);
	}
	close(fd);
	return (SUCCESS);
}

int	handle_redirects(t_cmd *cmd)
{
	t_redirect	*redirect;
	t_redirect	*last;
	int			status;

	if (!cmd || !cmd->redir)
		return (SUCCESS);
	last = find_last_heredoc(cmd->redir);
	redirect = cmd->redir;
	while (redirect)
	{
		status = process_redirect(redirect, last);
		if (status == FAILURE)
			return (FAILURE);
		redirect = redirect->next;
	}
	return (SUCCESS);
}
