/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:27:40 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/21 23:34:38 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "../src/libft/libft.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

# define SUCCESS 0
# define FAILURE 1

# define TRUE 1
# define FALSE 0

typedef struct s_cmd
{
	char			**args; //command and arguments
	char			*infile; // < file
	char			*outfile; // > file or >> file
	int				append; // 1 if >> append mode
	char			*heredoc_delim; // looks for "EOF" when << redirection is called in shell to tell the shell to stop reading from input
	struct s_cmd	*next; //linked list to next command
}	t_cmd;

typedef struct s_minishell
{
	t_cmd	*cmds; // linked list of parsed commands
	char	**envp; // environments variables
	int		exit_code; // last command exit code
}	t_minishell;

// Executor functions
int	execute_command(char **argv, char **envp);

// Builtin functions
int		is_builtin(char **argv);
int		run_builtin(char **argv, char **envp);
int		builtin_cd(t_cmd *cmd, t_minishell *shell);
int		builtin_pwd(t_cmd *cmd, t_minishell *shell);
int		builtin_echo(t_cmd *cmd, t_minishell *shell);
int		builtin_env(t_minishell *shell);
int		builtin_export(t_cmd *cmd, t_minishell *shell);
int		builtin_unset(t_cmd *cmd, t_minishell *shell);
int		builtin_exit(t_cmd *cmd, t_minishell *shell);
void	copy_envp(t_minishell *shell, char **envp);

//parsing functions
char	**tokenize_input(char *input);
t_cmd	*cmd_into_new_node(void);
char	**add_argument(char **args, char *arg);
t_cmd	*tokens_into_cmds(char **tokens);
char	*ft_strjoin_and_free(char *s1, char *s2);
char 	*get_env_value(char const *name, char **envp);
size_t  count_env(char **envp);
int 	cmp_env_names(char const *s1, char const *s2);
char    *get_var_name_end(char *ptr);

//debug parsing functions
void	print_debug_cmds(t_cmd *cmds);

# endif
