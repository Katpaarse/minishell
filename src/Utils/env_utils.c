/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:26:06 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/22 15:36:04 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// count number op environment variables
size_t  count_env(char **envp)
{
    size_t  i;

    if (!envp) // checks if the environment pointer array is NULL. If it is, there are 0 environment variables.
        return (0);
    i = 0;
    while (envp[i]) // iterates through the envp array until it reaches the NULL terminator. Each envp[i] is a string like "HOME=/Users/jukerste"
        i++;
    return (i); // returns the total count of environment variables
}

// function to get the value from the environment depending on search. Like "HOME" "USER" "PATH" etc
char    *get_env_value(char const *name, char **envp)
{
    int i;
    size_t len;

    if (!name || !envp)
        return (NULL);
    len = ft_strlen(name); // length of the variable name we’re searching for. Example: "HOME" → len = 4
    i = 0;
    while (envp[i]) // loop through each environment variable string
    {
        if (ft_strncmp(envp[i], name, len) == SUCCESS && envp[i][len] == '=') // checks if the first len characters match the variable name and the next character is '='. Example: For "HOME=/Users/jukerste", if name = "HOME", it matches
            return (ft_strdup(envp[i] + len + 1)); // Returns a new string with only the value of the variable. "HOME=/Users/jukerste" -> " /Users/jukerste"
        i++;
    }
    return (ft_strdup("")); // variable not found -> empty string
}

// return a strdup of the full "VAR=value" string, if found
char    *get_env_name(char const *name, char **envp)
{
    int     i;
    size_t  len;

    if (!name || !envp)
        return (NULL);
    len = ft_strlen(name);
    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], name, len) == SUCCESS && (envp[i][len] == '=' || envp[i][len] == '\0')) // checks if the string starts with the variable name. Then makes sure its either followed by '=' (normal variable) or '\0' (variable exists but no value)
            return (ft_strdup(envp[i])); // returns a full copy of the "VAR=value" string
        i++;
    }
    return (NULL);
}
// compare environment variables by name ("PATH=...") by name only. Stops when a '=' char is found
int cmp_env_names(char const *s1, char const *s2)
{
    while (*s1 && *s2 && *s1 != '=' && *s2 != '=' && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}
// find the end of a valid variable name in a string. Helps extract variable names from strings
char    *get_var_name_end(char *ptr)
{
    if (!ptr)
        return (NULL);
    if (!ft_isalpha(*ptr) && *ptr != '_') // checks if the first character is valid for a variable name: letter or underscore. $1VAR is invalid; stops immediately
        return (ptr);
    while (ft_isalnum(*ptr) || *ptr == '_') // loops until the character is no longer alphanumeric or underscore
        ptr++;
    return (ptr); // Returns the pointer to the first invalid character. Example: "HOME=/Users" -> returns pointer to '='
}
