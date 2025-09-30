/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 13:06:17 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/26 15:41:34 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// global variable definition
int g_minishell_is_executing = 0;
/*
wait, 
waitpid, 
wait3, 
wait4, 
signal,
sigaction, 
sigemptyset, 
sigaddset, 
kill, 
exit,
*/

// HANDLE GLOBAL VARIABLE FOR SIGINT
// 0 = waiting for user input (interactive mode)
// 1 = executing a command (child process)

void	handle_sigint(int signum)
{
	(void)signum;

	if (g_minishell_is_executing == 0) // only for interactive input
	{
		write(1, "\n", 1);
		// rl_free_line_state();
		// rl_cleanup_after_signal();
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		// rl_done = 1;	
	}
	else
	{
		write(1, "\n", 1);
	}
	// if g_minishell_is_executing is 1, the handler does nothing
	// and the SIGINT will simply interrupt the waitpid()
}

// void	handle_sigquit(int signum)
// {
// 	(void)signum;
// 	if (g_minishell_is_executing == 1)
// 		write(1, "Quit (core dumped)\n", 19);
// 	// Do nothing on SIGQUIT
// }

void	setup_signal_handlers(void)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	ft_memset(&sa_int, 0, sizeof(sa_int));
	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = 0; // Restart interrupted syscalls
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);

	ft_memset(&sa_quit, 0, sizeof(sa_quit));
	sa_quit.sa_handler = SIG_IGN; // Ignore SIGQUIT
	sa_quit.sa_flags = 0; // No special flags
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_child_signals(void)
{
	struct sigaction sa_default;
	// struct sigaction sa_quit;

	ft_memset(&sa_default, 0, sizeof(sa_default));
	sa_default.sa_handler = SIG_DFL; // Default action
	sa_default.sa_flags = 0; // No special flags
	sigemptyset(&sa_default.sa_mask);

	sigaction(SIGINT, &sa_default, NULL);
	sigaction(SIGQUIT, &sa_default, NULL);

	// ft_memset(&sa_quit, 0, sizeof(sa_quit));
	// sa_quit.sa_flags = 0;
	// sa_quit.sa_handler = SIG_DFL; // Default action
	// sigemptyset(&sa_quit.sa_mask);
	// sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_heredoc_signal_handlers(void)
{
	struct sigaction	sa_int;

	ft_memset(&sa_int, 0, sizeof(sa_int));
	sa_int.sa_handler = SIG_DFL;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
}
