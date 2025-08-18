/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:14 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/18 18:59:44 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
    pwd: pwd with no options requires 0 arguments.
        Example: pwd
*/

int	builtin_pwd(t_cmd *cmd, t_minishell *shell)
{
	char *cwd;

	(void)cmd; // Unused parameter, can be removed if not needed
	(void)shell; // Unused parameter, can be removed if not needed

	// Get the pathname of the current working directory,
	// 		extern char *getcwd (char *__buf, size_t __size) __THROW __wur;

	// Returns NULL if the directory couldn't be determined or SIZE was too small.
	// If successful, returns BUF.  In GNU, if BUF is NULL, an array is allocated with `malloc';
	// the array is SIZE bytes long, unless SIZE == 0, in which case it is as big as necessary.  

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd() error");
		return (FAILURE);
	}

	if (cmd->outfile != NULL)
	{
		int fd;
		int len;
	
		if (cmd->append)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0666); // '>>' append mode
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666); // '>' redirect mode

		if (fd < 0)
		{
			perror("Error opening outfile");
			free(cwd);
			return (FAILURE);
		}

		len = strlen(cwd);

		if (write(fd, cwd, len) < 0)
		{
			perror("Error writing to outfile");
			free(cwd);
		}
		if (write(fd, "\n", 1) < 0)
		{
			perror("Error writing newline to outfile");
			free(cwd);
		}

		free(cwd); // Free the allocated memory for cwd

		if (close(fd) < 0)
		{
			perror("Error closing outfile");
			return (FAILURE);
		}
		return (SUCCESS);
	}
	else
	{
		printf("%s\n", cwd);
		if (cwd)
			free(cwd); // Free the allocated memory for cwd
		return (SUCCESS);
	}
}
