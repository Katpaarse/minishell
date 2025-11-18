/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:23:25 by lavan-de          #+#    #+#             */
/*   Updated: 2025/11/18 13:58:58 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	t_minishell shell;
	int			i;
	char		*input;
	char		**tokens;
	char		*expanded;

	(void)argc;
	(void)argv;
	copy_envp(&shell, envp);
	setup_signal_handlers();
	shell.exit_code = 0;
	shell.cmds = NULL;
	
	while (1)
	{
		g_minishell_is_executing = 0;
		tokens = NULL;
		input = readline("minishell > ");
		if (!input)
		{
			clear_shell_cmds(&shell);
			rl_clear_history();
			if (g_minishell_is_executing == -1)
				shell.exit_code = 130;
			write(1, "exit\n", 5);
			break ; // if not input. Exit the loop
		}
		if (g_minishell_is_executing == -1)
		{
			shell.exit_code = 130;
			g_minishell_is_executing = 0;
		}
		if (ft_strlen(input) == 0 || is_whitespace_only(input))
		{
			free (input);
			shell.exit_code = 0;
			continue ;
		}
		if (input[0] != '\0') // if there is no empty input. Save it so history in shell
			add_history(input);
		tokens = tokenize_input(input);
		if (!tokens)
		{
			free(input); // frees the input line and goes back to the prompt without crashing
			shell.exit_code = 258;
			continue;
		}
	i = 0;
	while (tokens[i]) // loop through each token
	{
		// Skip processing heredoc delimiters - they need quotes preserved
		if (i > 0 && tokens[i - 1][0] == '<' && tokens[i - 1][1] == '<' 
			&& tokens[i - 1][2] == '\0')
		{
			i++;
			continue ;
		}
		// Normal variable expansion and quote removal for other tokens
		expanded = process_token(tokens[i], &shell);
		free(tokens[i]);
		tokens[i] = expanded;
		i++;
	}
		g_minishell_is_executing = 1;
		clear_shell_cmds(&shell);
		shell.cmds = tokens_into_cmds(tokens, &shell);
		g_minishell_is_executing = 0;
		if (!shell.cmds)
		{
			free_tokens(tokens);
			free(input);
			continue ;
		}
		free_tokens(tokens);
		tokens = NULL;
		free(input);
		input = NULL;
		if (shell.cmds)
		{
			g_minishell_is_executing = 1;
			execute_command(&shell);
			g_minishell_is_executing = 0;
			clear_shell_cmds(&shell);
		}
	}
	clear_shell_cmds(&shell);
	free_args(shell.envp);
	free_args(shell.exp_list);
	return (shell.exit_code);
}
