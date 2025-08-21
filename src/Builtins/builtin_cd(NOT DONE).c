/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:01 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/18 18:59:44 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// chdir
// getcwd
// write
// strerror
// malloc/free

/*
    cd: cd with a relative or absolute path requires exactly 1 argument. 
	The path is that argument.
        Example: cd /home, cd ../documents
*/

int	builtin_cd(t_cmd *cmd, t_minishell *shell, int fd)
{
	char *path;
	char *oldpwd;

	if (!cmd || !cmd->args || !shell)
		return (FAILURE);

	if (cmd->args[2] != NULL)
	{
		write(fd, "cd: too many arguments\n", 24);
		return (FAILURE);
	}

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (FAILURE);

	if (cmd->args[1] == NULL)
		path = getenv("HOME");
	else if (cmd->args[1] == "-")
		path = getenv("OLDPWD");
	else if (cmd->args[1] == "..")
		chdir("..");
	else if (cmd->args[1] == '.')
		chdir(".");
	else if (cmd->args[1] && cmd->args[1][0] == '/' && cmd->args[2] == NULL)
		path = cmd->args[1];

	if (path)
		chdir(path);

	// Update PWD and OLDPWD environment variables

	return (SUCCESS);
}
