/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 14:46:37 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/22 16:57:11 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_tokens_logic(t_minishell *shell, char **tokens)
{
	int		i;
	char	*expanded;

	i = 0;
	while (tokens[i])
	{
		if (i > 0 && tokens[i - 1][0] == '<' && tokens[i - 1][1] == '<'
			&& tokens[i - 1][2] == '\0')
		{
			i++;
			continue ;
		}
		expanded = process_token(tokens[i], shell);
		free(tokens[i]);
		tokens[i] = expanded;
		i++;
	}
}

char	**process_input_line(t_minishell *shell, char *input)
{
	char	**tokens;

	if (input[0] != '\0')
		add_history(input);
	tokens = tokenize_input(input);
	if (!tokens)
	{
		shell->exit_code = 258;
		return (NULL);
	}
	expand_tokens_logic(shell, tokens);
	return (tokens);
}

void	handle_empty_input(t_minishell *shell, char *input)
{
	free (input);
	shell->exit_code = 0;
}

int	check_eof_or_signal(t_minishell *shell, char **input)
{
	if (!*input)
	{
		write(1, "exit\n", 5);
		return (0);
	}
	if (g_minishell_is_executing == -1)
	{
		shell->exit_code = 130;
		g_minishell_is_executing = 0;
		return (1);
	}
	return (2);
}

void	cleanup_all(t_minishell *shell)
{
	clear_shell_cmds(shell);
	free_args(shell->envp);
	free_args(shell->exp_list);
	rl_clear_history();
}
