/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:58:19 by lavan-de          #+#    #+#             */
/*   Updated: 2025/10/28 12:28:37 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_whitespace_only(char *str)
{
	if (!str)
		return (FALSE);
	while (*str)
	{
		if (!is_space(*str))
			return (FALSE);	
		str++;
	}
	return (TRUE);
}

void	copy_envp(t_minishell *shell, char **envp)
{
	int	i;

	if (!envp || !shell)
		return ;
	i = 0;
	while (envp[i] != NULL)
		i++;
	shell->exp_list = malloc(sizeof(char *) * (i + 1));
	if (!shell->exp_list)
		return ;
	shell->envp = malloc(sizeof(char *) * (i + 1));
	if (!shell->envp)
	{
		free(shell->exp_list);
		shell->exp_list = NULL;
		return ;
	}
	i = 0;
	while (envp[i] != NULL)
	{
		shell->exp_list[i] = ft_strdup(envp[i]);
		shell->exp_list[i + 1] = NULL;
		if (!shell->exp_list[i])
		{
			free_args(shell->exp_list);
			free_args(shell->envp);
			shell->exp_list = NULL;
			shell->envp = NULL;
			return ;
		}
		shell->envp[i] = ft_strdup(envp[i]);
		shell->envp[i + 1] = NULL;
		if (!shell->envp[i])
		{
			free_args(shell->exp_list);
			free_args(shell->envp);
			shell->exp_list = NULL;
			shell->envp = NULL;
			return ;
		}
		i++;
	}
}

char    *ft_strjoin_and_free(char *s1, char *s2)
{
	size_t  len_s1;
	size_t  len_s2;
	char    *result;
	
	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	result = malloc(len_s1 + len_s2 + 1);
	if (!result)
	{
		free(s1);
		return (NULL);
	}
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

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_special_op(char c)
{
	return (c == '|' || c == '>' || c == '<');
}
