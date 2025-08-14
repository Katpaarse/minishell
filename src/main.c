/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:23:25 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/13 15:27:16 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **env)
{
    char    *cmd[] = {"/bin/ls", "-l", NULL};
    execute_command(cmd, envp);
    return 0;
}
