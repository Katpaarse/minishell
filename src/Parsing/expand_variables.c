/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:27:24 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/15 16:39:41 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_dollar(char const *input, int *i,
	t_minishell *shell, char *result)
{
	char	*dollar;

	if (input[*i] == '\0' || (!ft_isalpha(input[*i]) && input[*i]
			!= '_' && input[*i] != '?'))
	{
		dollar = ft_strdup("$");
		if (!dollar)
			return (free(result), NULL);
		result = ft_strjoin_and_free(result, dollar);
		return (result);
	}
	else if (input[*i] == '?')
	{
		result = expand_exit_code(result, shell->exit_code);
		(*i)++;
		return (result);
	}
	else
	{
		result = expand_variable(input, i, shell->envp, result);
		return (result);
	}
}

char	*expand_exit_code(char *result, int exit_code)
{
	char	*code;

	code = ft_itoa(exit_code);
	if (!code)
		return (result);
	result = ft_strjoin_and_free(result, code);
	free(code);
	return (result);
}

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
		return (result);
	value = get_env_value(name, envp);
	free(name);
	if (!value)
		return (result);
	joined = ft_strjoin_and_free(result, value);
	free(value);
	if (!joined)
		return (NULL);
	return (joined);
}

char	*expand_normal_char(char const *input, int *i, char *result)
{
	char	buffer[2];
	char	*dup;

	buffer[0] = input[*i];
	buffer[1] = '\0';
	dup = ft_strdup(buffer);
	result = ft_strjoin_and_free(result, dup);
	free(dup);
	(*i)++;
	return (result);
}

char	*variable_expansion(char const *input, t_minishell *shell)
{
	char	*result;
	int		i;
	int		q_single;
	int		q_double;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	q_single = 0;
	q_double = 0;
	i = 0;
	while (input[i])
	{
		update_quotes(input[i], &q_single, &q_double);
		if (input[i] == '$' && !q_single)
		{
			i++;
			result = handle_dollar(input, &i, shell, result);
		}
		else
			result = expand_normal_char(input, &i, result);
		if (!result)
			return (NULL);
	}
	return (result);
}
