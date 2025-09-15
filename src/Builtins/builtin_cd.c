/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:45:01 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/15 16:48:56 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
    cd: cd with a relative or absolute path requires exactly 1 argument. 
	The path is that argument.
        Example: cd /home, cd ../documents
*/

int	builtin_cd(t_cmd *cmd, t_minishell *shell)
{
	char	*path;
	char	*new_pwd;
	char	*old_pwd;
	int 	i;
	int		oldpwd_i;

	if (!cmd || !cmd->args || !shell)
		return (FAILURE);
	if (cmd->args[1] && cmd->args[2])
	{
		write(2, "cd: too many arguments\n", 24);
		return (FAILURE);
	}
	old_pwd = getcwd(NULL, 0); // MALLOC so free it later
	if (!old_pwd)
		return (FAILURE);

	if (cmd->args[1] == NULL)
		path = get_env_value("HOME", shell->envp);
	else if (ft_strncmp(cmd->args[1], "-", 2) == 0)
	{
		path = get_env_value("OLDPWD", shell->envp);
	}
	else	
		path = cmd->args[1];
	if (!path)
	{
		write(2, "cd: HOME/OLDPWD not set\n", 24);
		free(old_pwd);
		return (FAILURE);
	}
	if (chdir(path) != 0)
	{
		write(2, "cd: no such file or directory\n", 31);
		free(old_pwd);
		return (FAILURE);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		free(old_pwd);
		return (FAILURE);
	}
	oldpwd_i = -1;
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "PWD=", 4) == 0)
		{
			free(shell->envp[i]);
			shell->envp[i] = ft_strjoin("PWD=", new_pwd);
		}
		else if (ft_strncmp(shell->envp[i], "OLDPWD=", 7) == 0)
		{
			free(shell->envp[i]);
			shell->envp[i] = ft_strjoin("OLDPWD=", old_pwd);
			oldpwd_i = i;
		}
		i++;
	}
	if (cmd->args[1] && ft_strncmp(cmd->args[1], "-", 2) == 0 && oldpwd_i != -1)
	{
		write(1, shell->envp[oldpwd_i], ft_strlen(shell->envp[oldpwd_i]));
		write(1, "\n", 1);
	}
	free(new_pwd);
	free(old_pwd);
	return (SUCCESS);
}
/*
✅ Tests you should run in ./minishell

Run each of these in your minishell and compare to Bash. They’ll help check for segfaults and logic bugs:

Basic navigation

cd

Expect: changes to $HOME. No output.

Your code currently prints $HOME. → Check if that matches Bash.

cd /

Expect: changes to root /. No output.

Check pwd after.

cd ../

Expect: goes one directory up.

Error cases

cd doesnotexist

Expect: error message cd: no such file or directory. No segfault.

cd a b

Expect: error message cd: too many arguments.

OLDPWD handling

cd /tmp

cd -

Expect: goes back to the previous dir, prints its path.

cd - again

Expect: toggles back again.

Environment updates

pwd after cd /tmp

Expect: /tmp.

Run env | grep PWD

Expect: PWD=/tmp.

Run env | grep OLDPWD

Expect: OLDPWD=<previous dir>.

Unset env vars

Manually unset and then try cd:

unset HOME → then run cd.

Expect: error cd: HOME not set.

unset OLDPWD → then run cd -.

Expect: error cd: OLDPWD not set.

Stress cases

cd ""

Expect: error, no segfault.

cd .

Expect: stays in same directory, no output.

cd //

Should resolve to /.
*/
