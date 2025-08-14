/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:22:58 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/13 18:55:02 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* --BUILTINS--
echo (with -n option)

cd (change directory)

pwd (print working directory)

export (set environment variable)

unset (remove environment variable)

env (print environment variables)

exit (exit the shell)
--- --- --- --- --- --- --- --- ---
Everything else should be executed as an external program via 'execve'
*/


#include "../include/minishell.h"
#include <unistd.h>
#include <signal.h>

int execute_command(char **argv, char **envp)
{
    if (!argv || !argv[0] || argv[0][0] == '\0')
        return ();

    if (is_builtin(argv))
    {
        run_builtin(argv);
        return ();
    }

    fork();
    execve();
    waitpid();
}

int is_builtin(char **argv)
{
    if (argv[0])
    {
        if (ft_strncmp(argv[0], "cd", 3) == 0)
            return ();
        else if (ft_strncmp(argv[0], "echo", 5) == 0)
            return ();
        else if (ft_strncmp(argv[0], "pwd", 4) == 0)
            return ();
        else if (ft_strncmp(argv[0], "env", 4) == 0)
            return ();
        else if (ft_strncmp(argv[0], "export", 7) == 0)
            return ();
        else if (ft_strncmp(argv[0], "unset", 6) == 0)
            return ();
        else if (ft_strncmp(argv[0], "exit", 5) == 0)
            return ();
        else
            return ();
    }
    else
        return ();
}

int run_builtin(char **argv, char **envp)
{
    if (argv[0])
    {
        if (argv[0] = "cd")
        else if (argv[0] = "echo")
        else if (argv[0] = "pwd")
        else if (argv[0] = "env")
        else if (argv[0] = "export")
        else if (argv[0] = "unset")
        else if (argv[0] = "exit")
    }
    else
        return ();
}
