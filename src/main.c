/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:23:25 by lavan-de          #+#    #+#             */
/*   Updated: 2025/10/24 17:38:36 by jukerste         ###   ########.fr       */
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
	// initialize shell state
	copy_envp(&shell, envp); // store environment pointer to the struct
	setup_signal_handlers(); // setup signal handlers for SIGINT and SIGQUIT
	shell.exit_code = 0; // set not 0. So the last exit code is succes
	shell.cmds = NULL; // not parsed into commands yet
	
	while (1) // infinite loop untill user presses cntrl + D(EOF) or "exit" or gets out manually
	{
		g_minishell_is_executing = 0; // start in interactive mode
		input = readline("minishell > "); // shows minishell > and waiting for input
		if (!input)
		{
			if (g_minishell_is_executing == -1)
			{
				shell.exit_code = 130;
				g_minishell_is_executing = 0;
				continue ;
			}
			write(1, "exit\n", 5);
			break; // if not input. Exit the loop
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
			// skip expanding the token after << (heredoc delimiter)
			if (tokens[i][0] == '<' && tokens[i][1] == '<' && tokens[i][2] == '\0')
			{
				i = i + 2; // advance past << and its delimiter
				continue ;
			}
			// normal variable expansion and quote removal for other tokens
			expanded = process_token(tokens[i], &shell); // removes quotes from token input and calls variable expension
			free(tokens[i]);
			tokens[i] = expanded;
			i++;
		}
		g_minishell_is_executing = 1;
		shell.cmds = tokens_into_cmds(tokens, &shell);
		g_minishell_is_executing = 0;
		if (!shell.cmds)
		{
			i = 0;
			while (tokens[i])
			{
				free(tokens[i]);
				i++;
			}
			free(tokens);
			free(input);
			shell.exit_code = 258;
			continue ;
		}
		if (shell.cmds)
		{
			g_minishell_is_executing = 1;
			execute_command(&shell);
			g_minishell_is_executing = 0;
			cleanup_heredoc_files(shell.cmds->redirects);
			free_cmds(shell.cmds);
			shell.cmds = NULL;
		}
		i = 0;
		while (tokens[i]) // cleanup frees all the tokens and input
		{
			free(tokens[i]);
			i++;
		}
		free(tokens);
		free(input);
	}
	return (shell.exit_code);
}
