/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:06:47 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/15 18:07:28 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	copy_without_quotes(const char *token, char *result)
{
	int		i;
	int		j;
	char	in_quote;

	i = 0;
	j = 0;
	in_quote = 0;
	while (token[i])
	{
		if (!in_quote && (token[i] == '\'' || token[i] == '"'))
			in_quote = token[i];
		else if (in_quote && token[i] == in_quote)
			in_quote = 0;
		else
			result[j++] = token[i];
		i++;
	}
	result[j] = '\0';
}

char	*remove_quotes(const char *token)
{
	size_t	len;
	char	*result;

	if (!token)
		return (NULL);
	len = ft_strlen(token);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	copy_without_quotes(token, result);
	if (result[0] == '\0')
	{
		free(result);
		return (ft_strdup(""));
	}
	return (result);
}

char	*process_token(char *token, t_minishell *shell)
{
	size_t	len;
	char	*clean;
	char	*expanded;

	if (!token)
		return (NULL);
	len = ft_strlen(token);
	if (len >= 2 && token[0] == '\'' && token[len - 1] == '\'')
		return (remove_quotes(token));
	expanded = variable_expansion(token, shell);
	if (!expanded)
		return (NULL);
	clean = remove_quotes(expanded);
	free (expanded);
	return (clean);
}
