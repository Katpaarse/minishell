/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:23:25 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/17 14:34:04 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int main(int argc, char **argv, char **envp)
// {
//     char    *cmd[] = {"/bin/ls", "-l", NULL};
//     execute_command(cmd, envp);
//     return 0;
// }

int main(void)
{
    char    *input;
    char    **tokens;
    t_cmd   *cmds;

    input = "echo hello > out.txt | grep hi < in.txt";
    tokens = tokenize_input(input);
    cmds = tokens_into_cmds(tokens);
    print_debug_cmds(cmds);
    return (0);
}
