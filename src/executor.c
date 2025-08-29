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

// int main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;
//     char    *cmd[] = {"pwd", NULL};
//     execute_command(cmd, envp);
//     return (SUCCESS);
// }

int wait_for_child(pid_t pid)
{
    int status;

    waitpid(pid, &status, 0);
    return (status);
}

int execute_command(char **argv, char **envp, t_minishell *shell, t_cmd *cmd)
{
    if (!envp || !argv || !argv[0] || argv[0][0] == '\0')
        return (FAILURE);

    if (is_builtin(argv) == SUCCESS)
    {
        run_builtin(argv, envp);
        return (SUCCESS);
    }
	else
	{
		run_external(argv, envp);
		return (SUCCESS);
	}
}


int	run_external(char **argv, char **envp)
{
	pid_t	pid;
	int		status;
	char 	*cmd_path;

	cmd_path = find_cmd_path(argv, envp);
	pid = fork();

	if (pid == 0)
	{
		execve(cmd_path, argv, envp);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	return (status);
}

char	*find_cmd_path(char **argv, char **envp)
{
	char	*path;

	if (!argv || !argv[0] || argv[0][0] == '\0' || !envp)
		return (NULL);

	if (ft_strchr(argv[0], '/') != NULL)
		return (find_absolute_path(argv, envp));
	else 
	{
		path = NULL;
		path = find_relative_path(argv[0], path, envp);
		if (path)
			return (path);
	}

	return (NULL);
}

char	*find_absolute_path(char **argv, char **envp)
{
	int		i;
	char	*path;

	i = 0;
	if (!argv || !argv[0] || argv[0][0] == '\0' || !envp)
		return (NULL);
	if (ft_strchr(argv[0], '/') != NULL)
	{
		if (access(argv[0], X_OK) == 0)
			return (argv[0]);
		else
			return (NULL);
	}
	return (NULL);
}

char	*find_relative_path(char *cmd, char *path, char **envp)
{
	char	**paths;
	char	*full_path;
	int		i;
	
	if (!cmd || !path)
		return (NULL);

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = envp[i] + 5; // Skip "PATH="
			return (path);
		}
		i++;
	}

	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin_and_free(full_path, cmd);
		if (access(full_path, X_OK) == 0)
		{
			// Free allocated memory for paths
		}
	// Free allocated memory for paths
	}
}




