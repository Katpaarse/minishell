/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:27:24 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/26 14:10:16 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// join exit code into result string
char	*expand_exit_code(char *result, int exit_code)
{
	char	*code;
	
	code = ft_itoa(exit_code);
	if (code == NULL)
		return (result);
	result = ft_strjoin_and_free(result, code);
	free(code);
	return (result);
}

// starting at input[*i] (right after $), read a valid variable name and append its value
char	*expand_variable(char const *input, int *i, char **envp, char *result)
{
	int		start;
	char	*name;
	char	*value;
	
	start = *i;
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	name = ft_substr(input, start, *i - start);
	if (name == NULL)
		return (result);
	value = get_env_value(name, envp);
	free(name);
	if (value == NULL)
		return (result);
	result = ft_strjoin_and_free(result, value);
	free(value);
	return (result);
}
// copy one normal character (not part of $…) into result
char	*expand_normal_char(char const *input, int *i, char *result)
{
	char 	buffer[2];
	char	*dup;

	buffer[0] = input[*i];
	buffer[1] = '\0';
	dup = ft_strdup(buffer);
	result = ft_strjoin_and_free(result, dup);
	free(dup);
	(*i)++;
	return (result);
}
// It scans input left-to-right and decides what to do for each character
char    *expand_variables(const char *input, t_minishell *shell)
{
    char    *result;
    int     i;

    if (input == NULL)
        return (NULL);
    result = ft_strdup("");
    if (result == NULL)
        return (NULL);
    i = 0;
    while (input[i])
    {
        if (input[i] == '$')
        {
            i++;
            if (input[i] == '?')
            {
                result = expand_exit_code(result, shell->exit_code);
                i++;
            }
            else if (ft_isalpha(input[i]) || input[i] == '_')
                result = expand_variable(input, &i, shell->envp, result);
            else
                result = ft_strjoin_and_free(result, ft_strdup("$"));
        }
        else
            result = expand_literal_char(input, &i, result);
    }
    return (result);
}
