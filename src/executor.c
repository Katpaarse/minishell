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

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
    char    *cmd[] = {"pwd", NULL};
    execute_command(cmd, envp);
    return (SUCCESS);
}

int execute_command(char **argv, char **envp)
{
	(void)envp;
    if (!argv || !argv[0] || argv[0][0] == '\0')
        return (FAILURE);

    if (is_builtin(argv))
    {
        run_builtin(argv, envp);
        return (SUCCESS);
    }
	// else (EXTERNAL COMMAND)
	// Here you would typically fork a new process and use execve to run the command
	// For example:
	// if (fork() == 0) {
	//     execve(argv[0], argv, envp);
	//     perror("execve failed");
	//     exit(EXIT_FAILURE);
	// }
	// wait(NULL); // Wait for the child process to finish
	else
		return (FAILURE); // Placeholder for external command execution
    // fork();
    // execve();
    // waitpid();
}


int is_builtin(char **argv)
{
    if (argv[0])
    {
        if (ft_strncmp(argv[0], "cd", 3) == 0)
            return (SUCCESS);
		else if (ft_strncmp(argv[0], "echo", 5) == 0)
			return (SUCCESS);
        else if (ft_strncmp(argv[0], "pwd", 4) == 0)
			return (SUCCESS);
		else if (ft_strncmp(argv[0], "env", 4) == 0)
			return (SUCCESS);
        else if (ft_strncmp(argv[0], "export", 7) == 0)
            return (SUCCESS);
        else if (ft_strncmp(argv[0], "unset", 6) == 0)
            return (SUCCESS);
        else if (ft_strncmp(argv[0], "exit", 5) == 0)
            return (SUCCESS);
        else
            return (FAILURE);
    }
    else
        return (FAILURE);
}

int run_builtin(char **argv, char **envp)
{
    if (argv[0])
    {
        if (ft_strncmp(argv[0], "cd", 3) == 0)
            return (builtin_cd());
		else if (ft_strncmp(argv[0], "echo", 5) == 0)
			return (builtin_echo());
        else if (ft_strncmp(argv[0], "pwd", 4) == 0)
			return (builtin_pwd());
		else if (ft_strncmp(argv[0], "env", 4) == 0)
			return (builtin_env());
        else if (ft_strncmp(argv[0], "export", 7) == 0)
            return (builtin_export());
        else if (ft_strncmp(argv[0], "unset", 6) == 0)
            return (builtin_unset());
        else if (ft_strncmp(argv[0], "exit", 5) == 0)
            return (builtin_exit());
        else
            return (FAILURE);
    }
    else
        return (FAILURE);
}

... execute_external()
{}

... wait_for_child()
{}