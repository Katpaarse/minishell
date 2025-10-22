/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:14 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/21 19:36:12 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

	// Get the pathname of the current working directory,
	// 		extern char *getcwd (char *__buf, size_t __size) __THROW __wur;

	// Returns NULL if the directory couldn't be determined or SIZE was too small.
	// If successful, returns BUF.  In GNU, if BUF is NULL, an array is allocated with `malloc';
	// the array is SIZE bytes long, unless SIZE == 0, in which case it is as big as necessary.  

/*
    pwd: pwd with no options requires 0 arguments.
        Example: pwd
*/

int	builtin_pwd(void)
{
	char	*cwd;
	int		len;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		//error
		return (FAILURE);
	}
	len = strlen(cwd);
	if (write(1, cwd, len) < 0 || write(1, "\n", 1) < 0)
	{
		free(cwd);
		return (FAILURE);
	}
	if (cwd)
		free(cwd); // Free the allocated memory for cwd
	return (SUCCESS);
}
