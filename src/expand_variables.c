/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:27:24 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/23 18:29:27 by jukerste         ###   ########.fr       */
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
	return (result);
}

// extract variable name and expand it
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
	return (result);
}
// expand a single literal character
char	*expand_literal_char(char const *input, int *i, char *result)
{
	char buffer[2];

	buffer[0] = input[*i];
	buffer[1] = '\0';
	result = ft_strjoin_and_free(result, ft_strdup(buffer));
	(*i)++;
	return (result);
}
// main entry point
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
