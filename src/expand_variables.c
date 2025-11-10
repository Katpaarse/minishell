/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:27:24 by jukerste          #+#    #+#             */
/*   Updated: 2025/10/28 13:17:19 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// join exit code into result string. Handles this for $? as input in the shell
char	*expand_exit_code(char *result, int exit_code)
{
	char	*code;
	
	code = ft_itoa(exit_code); // converts the integer exit_code (from your shell struct) to a string
	if (!code)
		return (result);
	result = ft_strjoin_and_free(result, code); // append it to the new string. And frees the old string
	free(code); // free the temporary string
	return (result); // returns the updated string with the exit code appended
}

// starting at input[*i] (right after $), read a valid variable name and append its value
char	*expand_variable(char const *input, int *i, char **envp, char *result)
{
	int		start;
	char	*name;
	char	*value;
	
	start = *i; // stores the current index in the input string, right after $    Example: for $HOME, *i points to 'H'.
	while (ft_isalnum(input[*i]) || input[*i] == '_') // moves the pointer forward to read the full variable name. Stops when it hits a character that isn’t alphanumeric or underscore
		(*i)++;
	name = ft_substr(input, start, *i - start); // extract the variable name. Example: HOME from $HOME
	if (!name)
		return (result);
	value = get_env_value(name, envp); // looks up the environment variable. Returns an empty string or new string depending on the result
	free(name);
	if (value == NULL)
		return (result);
	result = ft_strjoin_and_free(result, value); // appends the variable value to the current result string
	free(value);
	return (result);
}
// copy one normal character (not part of $…) into result string
char	*expand_normal_char(char const *input, int *i, char *result)
{
	char 	buffer[2];
	char	*dup;

	buffer[0] = input[*i]; // stores the single character at the current index in a temporary string
	buffer[1] = '\0'; // null terminate the string with a single char
	dup = ft_strdup(buffer); // store the terminated string with a single char
	result = ft_strjoin_and_free(result, dup); // append the char to the result string
	free(dup);
	(*i)++; // move to the next char in the input
	return (result);
}
// It scans input left to right and decides what to do for each character
char    *expand_variables(const char *input, t_minishell *shell)
{
    char    *result;
    int     i;

    if (!input)
        return (NULL);
    result = ft_strdup(""); // initialize result as an empty string
    i = 0;
    while (input[i])
    {
        if (input[i] == '$') // if $ char is found. Variable expansion is needed
        {
			i++; // skip $ char first then check for ? or another char
			if (input[i] == '\0' || (!ft_isalpha(input[i]) && input[i] != '_' && input[i] != '?')) // // Lone $ or $ followed by invalid char - treat as literal $
			{
				char	*dollar;

				dollar = ft_strdup("$");
				if (!dollar)
				{
					free(result);
					return (NULL);
				}
				result = ft_strjoin_and_free(result, dollar);
				if (!result)
					return (NULL);
			}
            else if (input[i] == '?') 
            {
                result = expand_exit_code(result, shell->exit_code); // If next character is ? -> append last exit code using expand_exit_code
                i++;
            }
            else if (ft_isalpha(input[i]) || input[i] == '_') // If next character is a letter/underscore -> expand variable using expand_variable. Otherwise -> just copy $ literally
                result = expand_variable(input, &i, shell->envp, result);
            else
			{
				char	*dollar;

				dollar = ft_strdup("$");
				if (!dollar)
				{
					free(result);
					return (NULL);
				}
				result = ft_strjoin_and_free(result, dollar);
				if (!result)
					return (NULL);
			}
        }
        else
            result = expand_normal_char(input, &i, result);
    }
    return (result);
}
