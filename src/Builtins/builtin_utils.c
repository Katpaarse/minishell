/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 18:21:49 by lavan-de          #+#    #+#             */
/*   Updated: 2025/09/08 17:39:09 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*

Parent builtin with no pipes → run directly in parent.

Anything in a pipeline (even parent builtins) → fork, because that’s how pipelines are defined.

*/

// Must run in parent: cd, export, unset, exit -> they change shell state. Can run in child process: echo, pwd, env -> they output stuff
int	is_parent_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (FAILURE);
	if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		return (SUCCESS);
	if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (SUCCESS);
	if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		return (SUCCESS);
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		return (SUCCESS);
	return (FAILURE);
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


// CHECK OF DIT GEEN OVERLAP HEEFT MET DE "FORK" CODE VAN JULIAN, ZO JA 
// KUNNEN WE BETER EEN FORK FUNCTIE MAKEN DIE JE KUNT AANROEPEN IN PLAATS VAN DIT TE HERHALEN
// CHECK PIPELINE EXECUTIE
// CHECK EXTERNAL COMMAND
// CHECK CHILD BUILTIN EXECUTIE
// CHECK PARENT BUILTIN EXECUTIE
// CHECK REDIRECTIES
// CHECK WACHTEN OP CHILDREN

int run_builtin(t_cmd *cmd, t_minishell *shell)
{
    int saved_stdout = -1;
    int saved_stdin = -1;
    int result;
    
    if (!cmd->args[0])
        return (FAILURE);
    
    // Check if this is a parent builtin that needs to run in the parent process
    if (is_parent_builtin(cmd) == SUCCESS)
    {
        // Parent builtin - handle redirects without forking
        if (cmd->redirects)
        {
            // Save original file descriptors
            saved_stdout = dup(STDOUT_FILENO);
            saved_stdin = dup(STDIN_FILENO);
            
            if (saved_stdout < 0 || saved_stdin < 0)
            {
                if (saved_stdout >= 0) close(saved_stdout);
                if (saved_stdin >= 0) close(saved_stdin);
                return (FAILURE);
            }
        }
        
        // Apply redirects (if any)
        if (is_redirect(cmd, shell) == FAILURE)
        {
            if (saved_stdout >= 0) close(saved_stdout);
            if (saved_stdin >= 0) close(saved_stdin);
            return (FAILURE);
        }
        
        // Execute the builtin
        result = execute_builtin(cmd, shell);

        // Restore file descriptors if we redirected
        if (cmd->redirects)
        {
            if (saved_stdout >= 0)
            {
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdout);
            }
            if (saved_stdin >= 0)
            {
                dup2(saved_stdin, STDIN_FILENO);
                close(saved_stdin);
            }
        }
        
        return (result);
    }
    else
    {
        // Child builtin - fork if there are redirects, otherwise run in parent
        if (cmd->redirects)
        {
            pid_t pid = fork();
            if (pid < 0)
                return (1);
                
            if (pid == 0)
            {
                if (is_redirect(cmd, shell) == FAILURE)
                    exit(EXIT_FAILURE);

                result = execute_builtin(cmd, shell);
                exit(result);
            }
            
            int status;
            waitpid(pid, &status, 0);
            return (status / 256);
        }
        else
        {
            // No redirects, run normally in parent
            return (execute_builtin(cmd, shell));
        }
    }
}

int execute_builtin(t_cmd *cmd, t_minishell *shell)
{
    if (cmd->args[0])
    {
        if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
            return (builtin_cd(cmd, shell));
		else if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
			return (builtin_echo(cmd));
        else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
			return (builtin_pwd());
		else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
			return (builtin_env(shell));
        else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
            return (builtin_export(cmd, shell));
        else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
            return (builtin_unset(cmd, shell));
        else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
            return (builtin_exit(cmd, shell));
        else
            return (FAILURE);
    }
    else
        return (FAILURE);
}
