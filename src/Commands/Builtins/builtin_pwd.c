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

int	builtin_pwd(void)
{
	char	*cwd;
	int		len;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (FAILURE);
	len = strlen(cwd);
	if (write(1, cwd, len) < 0 || write(1, "\n", 1) < 0)
	{
		free(cwd);
		return (FAILURE);
	}
	if (cwd)
		free(cwd);
	return (SUCCESS);
}
