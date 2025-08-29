/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 18:21:49 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/21 19:36:26 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Summary Table
Builtin		Uses infile		Uses outfile/append
echo		❌				✅
cd			❌				❌
pwd			❌				✅
export		❌				⚠ optional (rare)
unset		❌				❌
env			❌				✅
exit		❌				❌
*/

/*int main(int argc, char **argv, char **envp)
{
    t_cmd cmd;
    t_minishell shell;
	*/
/*
    cmd.outfile = NULL;
    cmd.append = FALSE;
    cmd.args = NULL; // unused by pwd
    builtin_pwd(&cmd, &shell);

    cmd.outfile = "pwd_output.txt";
    cmd.append = FALSE;
    builtin_pwd(&cmd, &shell);
    printf("'pwd_output.txt' for output\n");
*/

    // echo hello world
    // char *args1[] = {"echo", "hellohihello", "world", NULL};
    // cmd.args = args1;
    // cmd.outfile = NULL;
    // cmd.append = FALSE;
    // builtin_echo(&cmd, &shell);
/*
    // echo -n hello world
    char *args2[] = {"-n", "hello", "world", NULL};
    cmd.args = args2;
    builtin_echo(&cmd, &shell);
*//*
    // echo hello world > echo_output.txt
    // cmd.outfile = "echo_output.txt";
    // cmd.append = TRUE;
    // builtin_echo(&cmd, &shell);
    // printf("Check file 'echo_output.txt' for output\n");
	int i = 0;
	while (shell.envp != NULL)
    {
		printf("%s\n", shell.envp[i]);
		i++;
		if (shell.envp[i] == NULL)
			break;
	}
	return (0);
}
*/

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

int run_builtin(char **argv, t_minishell *shell, t_cmd *cmd)
{
    if (argv[0])
    {
        if (ft_strncmp(argv[0], "cd", 3) == 0)
            return (builtin_cd(cmd, shell, /*fd*/));
		else if (ft_strncmp(argv[0], "echo", 5) == 0)
			return (builtin_echo(cmd));
        else if (ft_strncmp(argv[0], "pwd", 4) == 0)
			return (builtin_pwd());
		else if (ft_strncmp(argv[0], "env", 4) == 0)
			return (builtin_env(shell, /*fd*/));
        else if (ft_strncmp(argv[0], "export", 7) == 0)
            return (builtin_export(cmd, shell));
        else if (ft_strncmp(argv[0], "unset", 6) == 0)
            return (builtin_unset(cmd, shell));
        else if (ft_strncmp(argv[0], "exit", 5) == 0)
            return (builtin_exit(cmd, shell));
        else
            return (FAILURE);
    }
    else
        return (FAILURE);
}

int	is_redirect(t_cmd *cmd, t_minishell *shell)
{
	int fd;
	if (!cmd || !shell)
		return (FAILURE);

	if (cmd->outfile)
	{
		if (cmd->append == TRUE)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0666); // '>>' APPEND mode
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666); // '>' REDIRECT mode
		dup2(fd, 1); // Redirect stdout to outfile
	}
	else if (cmd->infile)
	{
		if (cmd->heredoc_delim == TRUE)
		{
			return (FAILURE);
		}
		else
		{
			fd = open(cmd->infile, O_RDONLY);
		}
		dup2(fd, 0); // Redirect stdin to infile
	}
	else
		return (FAILURE);

	if (fd > 0)
		close(fd);
	else if (fd < 0)
	{
		perror("Error opening infile/outfile");
		return (FAILURE);
	}

	return (SUCCESS);
}

void	copy_envp(t_minishell *shell, char **envp)
{
	int i;

	i = 0;
	if (!envp || !shell)
		return;

	// Count the number of environment variables
	while (envp[i] != NULL)
		i++;
	
	shell->envp = malloc(sizeof(char *) * (i + 1));
	if (!shell->envp)
	{
		perror("envp malloc failed");
		exit(1);
	}

	i = 0;
	while (envp[i] != NULL)
	{
		shell->envp[i] = ft_strdup(envp[i]);
		if (!shell->envp[i])
		{
			perror("ft_strdup failed");
			exit(1);
		}
		i++;
	}
	shell->envp[i] = NULL; // Null-terminate the array
}

