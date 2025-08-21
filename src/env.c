/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:26:06 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/21 18:25:05 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function the get the value from the environment depending on search. Like "HOME" "USER" "PATH" etc
char *get_value_env(char const *name, char **envp)
{
    int i;
    size_t len;

    if (name == NULL || envp == NULL)
        return (NULL);
    len = ft_strlen(name);
    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], name, len) == SUCCESS && envp[i][len] == '=')
            return (ft_strdup(envp[i] + len + 1)); // return value after the '=' char
        i++;
    }
    return (ft_strdup("")); // variable not found → empty string
}
