/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:06:47 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/22 15:31:50 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function to remove quotes from token. Token at this stage are just single chars
char	*remove_quotes(char const *token)
{
	size_t	len;
	
	if (!token)
		return (NULL);
	len = ft_strlen(token);
	if (len < 2)
		return (ft_strdup(token)); // token is too short to have matching quotes. So just copy the token as is
	if (token[0] != '\'' && token[0] != '"')
		return (ft_strdup(token)); // token with no quotes. So just copy the token as is
	if (token[len - 1] != token[0])
		return (ft_strdup(token)); // if the token doesnt have matching quotes before '\0' at the end compared to first char which should be quote. Just copy the token as is
	return (ft_substr(token, 1, len - 2)); // quotes removed from token string
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
	{
		remove_quotes(token);
		return (token);
	}
	expanded = expand_variables(token, shell); // the function returns a malloc'ed string. So we need to free it after use
	if (!expanded)
		return (NULL);
	clean = remove_quotes(expanded); // remove quotes if double quotes are found after variable expansion
	free (expanded);
	return (clean); // return the new token without quotes
}
