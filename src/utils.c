/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:58:19 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/20 16:25:48 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *ft_strjoin_and_free(char *s1, char *s2)
{
    size_t  len_s1;
    size_t  len_s2;
    char    *result;
    if (s1 == NULL && s2 == NULL)
        return (NULL);
	if (s1 == NULL)
		return (ft_strdup(s2));
	if (s2 == NULL)
		return (ft_strdup(s1));
    len_s1 = ft_strlen(s1);
    len_s2 = ft_strlen(s2);
    result = malloc(len_s1 + len_s2 + 1);
    if (result == NULL)
        return (NULL);
    ft_memcpy(result, s1, len_s1);
    free(s1);
    ft_memcpy(result + len_s1, s2, len_s2);
    free(s2);
    result[len_s1 + len_s2] = '\0';
    return (result);
}
