/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:27:24 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/22 15:47:20 by jukerste         ###   ########.fr       */
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
	
}