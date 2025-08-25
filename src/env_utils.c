/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:26:06 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/25 16:00:26 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// count number op environment variables
size_t  count_env(char **envp)
{
    size_t  i;

    if (envp == 0)
        return (0);
    i = 0;
    while (envp[i])
        i++;
    return (i);
}

// function to get the value from the environment depending on search. Like "HOME" "USER" "PATH" etc
char    *get_env_value(char const *name, char **envp)
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
    return (ft_strdup("")); // variable not found -> empty string
}

// return a strdup of the full "VAR=value" string, if found
char    *get_env_name(char const *name, char **envp)
{
    int     i;
    size_t  len;

    if (name == NULL || envp == NULL)
        return (NULL);
    len = ft_strlen(name);
    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], name, len) == SUCCESS && (envp[i][len] == '=' || envp[i][len] == '\0'))
            return (ft_strdup(envp[i]));
        i++;
    }
    return (NULL);
}
// compare environment variables by name ("PATH=...") by name only 
int cmp_env_names(char const *s1, char const *s2)
{
    while (*s1 && *s2 && *s1 != '=' && *s2 != '=' && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}
// find the end of a valid variable name in a string
char    *get_var_name_end(char *ptr)
{
    if (ptr == NULL)
        return (NULL);
    if (!ft_isalpha(*ptr) && *ptr != '_')
        return (ptr);
    while (ft_isalnum(*ptr) || *ptr == '_')
        ptr++;
    return (ptr);
}
