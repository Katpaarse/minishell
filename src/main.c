/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:23:25 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/26 17:43:24 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(void)
{
    t_minishell shell;
    char *input;
    char **tokens;
    int i;

    // Setup example environment
    char *envp[] = {"USER=francis", "HOME=/home/francis", "PATH=/usr/bin", NULL};
    shell.envp = envp;
    shell.exit_code = 42;

    input = "echo $USER $? $HOME hello";

    // Tokenize
    tokens = tokenize_input(input);

    printf("Tokens:\n");
    i = 0;
    while (tokens[i])
    {
        printf("[%s]\n", tokens[i]);
        i++;
    }

    // Expand variables
    printf("\nExpanded tokens:\n");
    i = 0;
    while (tokens[i])
    {
        char *expanded = expand_variables(tokens[i], &shell);
        printf("[%s]\n", expanded);
        free(expanded);
        i++;
    }

    // Free tokens
    i = 0;
    while (tokens[i])
        free(tokens[i++]);
    free(tokens);

    return 0;
}
