/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 14:46:25 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/22 16:45:12 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ctrl_c_shell(t_minishell *shell)
{
	if (g_minishell_is_executing == -1)
	{
		shell->exit_code = 130;
		g_minishell_is_executing = 0;
	}
}

void	shell_init(t_minishell *shell, char **envp)
{
	copy_envp(shell, envp);
	setup_signal_handlers();
	shell->exit_code = 0;
	shell->cmds = NULL;
}

void	execute_and_free(t_minishell *shell, char **tokens, char *input)
{
	g_minishell_is_executing = 1;
	clear_shell_cmds(shell);
	shell->cmds = tokens_into_cmds(tokens, shell);
	g_minishell_is_executing = 0;
	free_tokens(tokens);
	free(input);
	if (!shell->cmds)
		return ;
	g_minishell_is_executing = 1;
	execute_command(shell);
	g_minishell_is_executing = 0;
	clear_shell_cmds(shell);
}

int	loop_iteration_logic(t_minishell *shell)
{
	char	*input;
	char	**tokens;
	int		status;

	g_minishell_is_executing = 0;
	tokens = NULL;
	input = readline("minishell > ");
	ctrl_c_shell(shell);
	status = check_eof_or_signal(shell, &input);
	if (status == 0)
		return (0);
	if (status == 1)
		return (1);
	if (ft_strlen(input) == 0 || is_whitespace_only(input))
	{
		handle_empty_input(shell, input);
		return (1);
	}
	tokens = process_input_line(shell, input);
	if (!tokens)
		return (free(input), 1);
	execute_and_free(shell, tokens, input);
	return (2);
}

void	run_minishell_loop(t_minishell *shell)
{
	int	status;

	while (1)
	{
		status = loop_iteration_logic(shell);
		if (status == 0)
			break ;
		if (status == 1)
			continue ;
	}
	cleanup_all(shell);
}
