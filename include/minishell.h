/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:27:40 by jukerste          #+#    #+#             */
/*   Updated: 2025/09/24 18:22:02 by jukerste         ###   ########.fr       */
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
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

# define SUCCESS 0
# define FAILURE 1

# define TRUE 1
# define FALSE 0

// Dit is nodig om meerdere redirections in 1 command te kunnen bijhouden en uitvoeren.
// Bijvoorbeeld: cmd < infile > outfile >> appendfile

/*
VOORBEELD:
command: cat < in.txt > out.txt >> log.txt 

cmd->args = ["cat", NULL]
cmd->redirects[0] = { "in.txt", RED_INPUT }
cmd->redirects[1] = { "out.txt", RED_OUTPUT }
cmd->redirects[2] = { "log.txt", RED_APPEND }

*/

typedef enum e_redirect_type
{
	RED_NONE,
	RED_INPUT,      // <
	RED_OUTPUT,     // >
	RED_APPEND,     // >>
	RED_HEREDOC     // <<
}	t_redirect_type;

typedef struct s_redirect
{
	char 					*filename;		// name of the file for redirection
	t_redirect_type 		type;			// type of redirection
	struct s_redirect		*next;
}	t_redirect;

typedef struct s_cmd
{
	char			**args; //command and arguments
	t_redirect		*redirects; // array of redirections
	struct s_cmd	*next; //linked list to next command
}	t_cmd;

typedef struct s_minishell
{
	t_cmd	*cmds; // linked list of parsed commands
	char	**envp; // environments variables
	char	**exp_list; // exported variables list
	int		exit_code; // last command exit code
}	t_minishell;

// Executor functions
int 	wait_for_child(pid_t pid);
int		run_external(t_cmd *cmd, t_minishell *shell);
char	*find_cmd_path(char **argv, char **envp);
char	*find_absolute_path(char **argv);
char	*find_relative_path(char *cmd, char **envp);
int 	execute_command(t_minishell *shell);

// functions for handling pipes
void 	wait_all_children(t_minishell *shell, pid_t *child_pids, int count);
pid_t 	fork_and_execute_child(t_minishell *shell, t_cmd *current, int prev_fd, int *fd);
void	execute_pipeline(t_minishell *shell, t_cmd *cmds);
void 	execute_child(t_minishell *shell, t_cmd *current, int prev_fd, int *fd);

// Builtin functions
int		builtin_cd(t_cmd *cmd, t_minishell *shell);
int		builtin_pwd(void);
int		builtin_echo(t_cmd *cmd);
int		builtin_env(t_minishell *shell);
int 	builtin_export(t_cmd *cmd, t_minishell *shell);
int		builtin_unset(t_cmd *cmd, t_minishell *shell);
int		builtin_exit(t_cmd *cmd, t_minishell *shell);

// Builtin helper functions
int 	execute_builtin(t_cmd *cmd, t_minishell *shell);
int		is_builtin(char **argv);
int		run_builtin(t_cmd *cmd, t_minishell *shell);
int		is_redirect(t_cmd *cmd, t_minishell *shell);
void	copy_envp(t_minishell *shell, char **envp);
int		is_parent_builtin(t_cmd *cmd);
void	add_or_update_exp(t_minishell *shell, char *var);
void	add_or_update_env(t_minishell *shell, char *var);

//parsing functions
char		**tokenize_input(char *input);
t_cmd		*cmd_into_new_node(void);
char		**add_argument(char **args, char *arg);
t_cmd		*tokens_into_cmds(char **tokens, t_minishell *shell);
char		*ft_strjoin_and_free(char *s1, char *s2);
char		**ft_arrayjoin_and_free(char **arr, char *str);
char 		*get_env_value(char const *name, char **envp);
size_t  	count_env(char **envp);
char    	*get_env_name(char const *name, char **envp);
int 		cmp_env_names(char const *s1, char const *s2);
char    	*get_var_name_end(char *ptr);
char		*expand_exit_code(char *result, int exit_code);
char		*expand_variable(char const *input, int *i, char **envp, char *result);
char		*expand_normal_char(char const *input, int *i, char *result);
char    	*expand_variables(const char *input, t_minishell *shell);
char		*remove_quotes(char const *token);
char		*process_token(char *token, t_minishell *shell);
void		print_syntax_error(t_minishell *shell, char const *token);
void		print_error(t_minishell *shell, char const *message);
int			count_redirects(t_redirect *list);
t_redirect	*add_redirect(t_redirect *list, char *filename, t_redirect_type type);
void		free_redirects(t_redirect *list);
void		free_args(char **args);
void		free_cmds(t_cmd *cmd);
char		*handle_heredoc(char const *delimiter, int i);

# endif
