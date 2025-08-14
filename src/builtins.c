/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 18:21:49 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/13 18:22:09 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


... builtin_echo()
{}

int	builtin_pwd(void)
{
	char *cwd;
/* 
	Get the pathname of the current working directory,
		extern char *getcwd (char *__buf, size_t __size) __THROW __wur;

	Returns NULL if the directory couldn't be determined or SIZE was too small.
	If successful, returns BUF.  In GNU, if BUF is NULL, an array is allocated with `malloc';
	the array is SIZE bytes long, unless SIZE == 0, in which case it is as big as necessary.  
*/
	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		printf("%s\n", cwd);
		free(cwd);
		return (SUCCESS);
	}
	else
	{
		perror("getcwd() error");
		return (FAILURE);
	}
}

... builtin_exit()
{}
