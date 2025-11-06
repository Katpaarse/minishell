/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:27:40 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/06 13:34:07 by jukerste         ###   ########.fr       */
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
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>

# define SUCCESS 0
# define FAILURE 1

# define TRUE 1
# define FALSE 0

extern volatile sig_atomic_t g_minishell_is_executing;		// 0 = waiting for user input (interactive mode)
															// 1 = executing a command (child process)
															// < 0 = signal received
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
	int						expand;			// 1 if variable expansion is allowed, 0 if quoted delimiter
	struct s_redirect		*next;
}	t_redirect;

typedef struct s_cmd
{
	char			**args; //command and arguments
	t_redirect		*redir; // array of redirections
	struct s_cmd	*next; //linked list to next command
}	t_cmd;

typedef struct s_minishell
{
	t_cmd	*cmds; // linked list of parsed commands
	char	**envp; // environments variables
	char	**exp_list; // exported variables list
	int		exit_code; // last command exit code
}	t_minishell;

typedef struct s_parsing
{
	t_cmd		*current;
	char		**tokens;
	int			i;
	int			hi;
	t_minishell	*shell;
}	t_parsing;

// Free functions
void	free_redirects(t_redirect *redirect);
void	free_args(char **args);
void	free_cmds(t_cmd *cmd);
void	free_minishell(t_minishell *shell);

// Signal handling functions
void	setup_signal_handlers(void);
void	setup_child_signals(void);
void	handle_sigint(int signum);
void	setup_heredoc_signal_handlers(void);

// Executor functions
int 	wait_for_child(pid_t pid);
int		run_external(t_cmd *cmd, t_minishell *shell);
char	*find_cmd_path(char **argv, char **envp);
char	*find_absolute_path(char **argv);
char	*find_relative_path(char *cmd, char **envp);
int 	execute_command(t_minishell *shell);
int		execute_path_cmd(t_cmd *cmd, t_minishell *shell);
int		cmd_fork(t_cmd *cmd, t_minishell *shell, char *cmd_path);

// functions for handling pipes
int 	wait_all_children(t_minishell *shell, pid_t *child_pids, int count);
pid_t 	fork_and_execute_child(t_minishell *shell, t_cmd *current, int prev_fd, int *fd);
void	execute_pipeline(t_minishell *shell, t_cmd *cmds);
void 	execute_child(t_minishell *shell, t_cmd *current, int prev_fd, int *fd);

// Builtin cd
int		builtin_cd(t_cmd *cmd, t_minishell *shell);
int		new_old_pwd(t_cmd *cmd, t_minishell *shell);
char	*change_dir(t_cmd *cmd, t_minishell *shell, char *old_pwd);
void	set_pwd_env(t_cmd *cmd, t_minishell *shell, char *n_p, char *o_p);

// Builtin echo
int		builtin_echo(t_cmd *cmd);
int 	skip_nl(t_cmd *cmd, int i);
void	write_echo(t_cmd *cmd, int i);

// Builtin env
int		builtin_env(t_minishell *shell);

// Builtin exit
int		builtin_exit(t_cmd *cmd, t_minishell *shell);

// Builtin export
int 	builtin_export(t_cmd *cmd, t_minishell *shell);
void	write_export(t_minishell *shell);

// Builtin pwd
int		builtin_pwd(void);

// Builtin unset
int		builtin_unset(t_cmd *cmd, t_minishell *shell);

//
int		handle_redirects(t_cmd *cmd);

// Builtin helper functions
int 	execute_builtin(t_cmd *cmd, t_minishell *shell);
int		is_builtin(t_cmd *cmd);
int		run_builtin(t_cmd *cmd, t_minishell *shell);
void	copy_envp(t_minishell *shell, char **envp);
int		is_parent_builtin(t_cmd *cmd);
void	add_or_update_exp(t_minishell *shell, char *var);
void	add_or_update_env(t_minishell *shell, char *var);

//parsing functions
char		**tokenize_input(char *input);
int			handle_pipe_token(t_parsing *p);
int			handle_redirect_token(t_parsing *p);
int			handle_heredoc_wrapper(t_parsing *p);
int			handle_heredoc_token(t_cmd *current, char *raw_token, int heredoc_index, t_minishell *shell);
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
void		print_syntax_error(t_minishell *shell, char const *token);
void		print_error(t_minishell *shell, char const *message);
void		print_error_filename(char const	*filename, char	const *message);
int			count_redirects(t_redirect *list);
t_redirect	*add_redirect(t_redirect *list, char *filename, t_redirect_type type, int expand);
void		free_redirects(t_redirect *list);
void		free_args(char **args);
void		free_cmds(t_cmd *cmd);
void		handle_heredoc(char const *delimiter, char	*tmpfile, t_minishell *shell, int expand);
char 		*process_heredoc(char const *delimiter, int i, t_minishell *shell, int expand);
char		*process_token(char *token, t_minishell *shell);
void		cleanup_heredoc_files(t_redirect *redirects);
int			is_space(char c);
int			is_whitespace_only(char *str);
int			is_special_op(char c);

# endif
