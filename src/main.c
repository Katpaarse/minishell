/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:23:25 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/30 15:21:58 by jukerste         ###   ########.fr       */
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
	g_minishell_is_executing = 0; // start in interactive mode
	
	while (1) // infinite loop untill user presses cntrl + D(EOF) or "exit" or gets out manually
	{
		input = readline("minishell > "); // shows minishell > and waiting for input
		if (!input)
		{
			if (g_minishell_is_executing == 0)
			{
				write(1, "exit\n", 5);
				break; // if not input. Exit the loop
			}
			else
			{
				printf("else TEST\n");
				g_minishell_is_executing = 0;
				continue ;
			}
		}
		if (input[0] != '\0') // if there is no empty input. Save it so history in shell
			add_history(input);
		tokens = tokenize_input(input);
		if (!tokens)
		{
			free(input); // frees the input line and goes back to the prompt without crashing
			continue;
		}
		i = 0;
		while (tokens[i]) // loop through each token
		{
			expanded = expand_variables(tokens[i], &shell); // expand_variables replaces $HOME with its value from envp
			free(tokens[i]);
			tokens[i] = expanded;
			i++;
		}
		shell.cmds = tokens_into_cmds(tokens, &shell);
		if (shell.cmds)
		{
			g_minishell_is_executing = 1;
			shell.exit_code = execute_command(&shell);
			g_minishell_is_executing = 0;
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
