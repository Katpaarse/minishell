/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavan-de <lavan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 18:21:49 by lavan-de          #+#    #+#             */
/*   Updated: 2025/08/18 18:59:37 by lavan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (envp[i] != NULL)
	{
		shell->envp[i] = ft_strdup(envp[i]);
		if (!shell->envp[i])
		{
			perror("ft_strdup failed");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	shell->envp[i] = NULL; // Null-terminate the array
}

