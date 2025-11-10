/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:27:24 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/10 16:26:37 by jukerste         ###   ########.fr       */
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
	char	*joined;

	start = *i;
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	name = ft_substr(input, start, *i - start);
	if (!name)
		return (result); // We cannot safely free result here
	value = get_env_value(name, envp);
	free(name);

	if (!value) // No variable found → no change
		return (result);
	joined = ft_strjoin_and_free(result, value);
	free(value);
	if (!joined) // join failed → both result and value are already freed
		return (NULL);
	return (joined);
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
	char	*result;
	char	*dollar;
	int		i;

	if (!input)
		return (NULL);
	result = ft_strdup(""); // initialize result as an empty string
	if (!result)
		return (NULL);
	i = 0;
	while (input[i])
	{
		if (input[i] == '$')
		{
			i++; // skip '$'
			// Case 1: "$" with no valid variable follows → literal '$'
			if (input[i] == '\0'
				|| (!ft_isalpha(input[i]) && input[i] != '_' && input[i] != '?'))
			{
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
			// Case 2: "$?"
			else if (input[i] == '?')
			{
				result = expand_exit_code(result, shell->exit_code);
				if (!result)
					return (NULL);
				i++;
			}
			// Case 3: "$VAR"
			else if (ft_isalpha(input[i]) || input[i] == '_')
			{
				result = expand_variable(input, &i, shell->envp, result);
				if (!result)
					return (NULL);
			}
		}
		else
		{
			result = expand_normal_char(input, &i, result);
			if (!result)
				return (NULL);
		}
	}
	return (result);
}
