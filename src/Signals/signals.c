/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 13:06:17 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/24 13:06:19 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	handle_sigint(int signum)
{
	(void)signum;

	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int signum)
{
	(void)signum;
	// Do nothing on SIGQUIT
}

void	setup_signal_handlers(void)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	sa_int.sa_flags = SA_RESTART; // Restart interrupted syscalls
	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);

	sa_quit.sa_flags = SA_RESTART;
	sa_quit.sa_handler = handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);

	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_child_signals(void)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	sa_int.sa_flags = 0; // No special flags
	sa_int.sa_handler = SIG_DFL; // Default action
	sigemptyset(&sa_int.sa_mask);

	sa_quit.sa_flags = 0;
	sa_quit.sa_handler = SIG_DFL; // Default action
	sigemptyset(&sa_quit.sa_mask);

	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}
