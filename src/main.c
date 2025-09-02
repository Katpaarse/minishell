/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:23:25 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/02 16:37:54 by jukerste         ###   ########.fr       */
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
	shell.envp = envp; // store environment pointer to the struct
	shell.exit_code = 0; // set not 0. So the last exit code is succes
	shell.cmds = NULL; // not parsed into commands yet
	
	while (1) // infinite loop untill user presses cntrl + D(EOF) or gets out manually
	{
		input = readline("minishell > "); // shows minishell > and waiting for input
		if (input == NULL) 
			break; // if not input. Exit the loop
		if (input[0] != '\0') // if there is no empty input. Save it so history in shell
			add_history(input);
		tokens = tokenize_input(input);
		if (tokens == NULL)
		{
			free(input); // frees the input line and goes back to the prompt without crashing
			continue;
		}
		i = 0;
		while (tokens[i]) // loop through each token
		{
			expanded = expand_variables(tokens[i], &shell); // expand_variables replaces $HOME with its value from envp
			free(expanded);
			i++;
		}
		if (shell.cmds)
		{
			execute_command(&shell);
			return (0);
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
	return (0);
}
