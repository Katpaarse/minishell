/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jul <jul@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:06:47 by jukerste          #+#    #+#             */
/*   Updated: 2025/10/22 20:26:34 by jul              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function to remove quotes from token. Token at this stage are just single chars
char	*remove_quotes(char const *token)
{
	size_t	len;
	char	*result;
	int		i;
	int		j;
	char	in_quote;

	if (!token)
		return (NULL);
	
	len = ft_strlen(token);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	
	i = 0;
	j = 0;
	in_quote = 0;
	
	while (token[i])
	{
		if (!in_quote && (token[i] == '\'' || token[i] == '"'))
		{
			// Start of a quote - remember which type and skip it
			in_quote = token[i];
		}
		else if (in_quote && token[i] == in_quote)
		{
			// End of the current quote - skip it
			in_quote = 0;
		}
		else
		{
			// Regular character or inside quotes - copy it
			result[j++] = token[i];
		}
		i++;
	}
	result[j] = '\0';
	// If we removed everything, return empty string instead of freeing
	if (j == 0)
	{
		free(result);
		return (ft_strdup(""));
	}
	return (result);
}

// function to process the new token with or without quotes. Only when double quotes are found then expansion is needed
char	*process_token(char *token, t_minishell *shell)
{
	size_t	len;
	char	*clean;
	char	*expanded;

	if (!token)
		return (NULL);
	len = ft_strlen(token);
	if (len >= 2 && token[0] == '\'' && token[len - 1] == '\'') // single matching quotes in token. No expansion needed. Just remove single quotes
		return (remove_quotes(token));
	expanded = expand_variables(token, shell); // the function returns a malloc'ed string. So we need to free it after use
	if (!expanded)
		return (NULL);
	clean = remove_quotes(expanded); // remove quotes if double quotes are found after variable expansion
	free (expanded);
	return (clean); // return the new token without quotes
}
