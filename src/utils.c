/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:58:19 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/09 18:56:35 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	copy_envp(t_minishell *shell, char **envp)
{
	int i;

	if (!envp || !shell)
		return;
	// Count the number of environment variables
	i = 0;
	while (envp[i] != NULL)
		i++;
	
	shell->envp = malloc(sizeof(char *) * (i + 1));
	if (!shell->envp)
		print_error(shell,"envp malloc failed");
	i = 0;
	while (envp[i] != NULL)
	{
		shell->envp[i] = ft_strdup(envp[i]);
		if (!shell->envp[i])
			print_error(shell, "ft_strdup failed");
		i++;
	}
	shell->envp[i] = NULL; // Null-terminate the array
}

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
	ft_memcpy(result + len_s1, s2, len_s2);
	result[len_s1 + len_s2] = '\0';
	free(s1);
	return (result);
}

char	**ft_arrayjoin_and_free(char **arr, char *str)
{
	int		i;
	int		count;
	char	**new_arr;

	count = 0;
	while (arr && arr[count])
		count++;
	new_arr = malloc(sizeof(char *) * (count + 2));
	if (new_arr == NULL)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_arr[i] = arr[i];
		i++;
	}
	new_arr[i] = str;
	new_arr [i + 1] = NULL;
	free(arr);
	return (new_arr);
}
