/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:27:40 by jukerste          #+#    #+#             */
/*   Updated: 2025/11/20 18:18:49 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>

# define SUCCESS 0
# define FAILURE 1

# define TRUE 1
# define FALSE 0

extern volatile sig_atomic_t	g_minishell_is_executing;

typedef enum e_redirect_type
{
	RED_NONE,
	RED_INPUT,
	RED_OUTPUT,
	RED_APPEND,
	RED_HEREDOC
}	t_redirect_type;

typedef struct s_redirect
{
	char					*filename;
	t_redirect_type			type;
	struct s_redirect		*next;
}	t_redirect;

typedef struct s_cmd
{
	char			**args;
	t_redirect		*redir;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_minishell
{
	t_cmd	*cmds;
	char	**envp;
	char	**exp_list;
	int		exit_code;
}	t_minishell;

typedef struct s_parsing
{
	t_cmd		*current;
	char		**tokens;
	int			i;
	int			hi;
	t_minishell	*shell;
}	t_parsing;

/* ************************************************************************** */
/*                              FREE FUNCTIONS                                */
/* ************************************************************************** */
void		free_redirects(t_redirect *redirect);
void		free_args(char **args);
void		free_cmds(t_cmd *cmd);
void		free_minishell(t_minishell *shell);
void		free_tokens(char **tokens);
void		clear_shell_cmds(t_minishell *shell);

/* ************************************************************************** */
/*                             SIGNAL HANDLING                                */
/* ************************************************************************** */
void		setup_signal_handlers(void);
void		setup_child_signals(void);
void		setup_heredoc_signal_handlers(void);
void		handle_sigint(int signum);

/* ************************************************************************** */
/*                                 EXECUTOR                                   */
/* ************************************************************************** */
int			execute_command(t_minishell *shell);
int			execute_path_cmd(t_cmd *cmd, t_minishell *shell);
int			run_external(t_cmd *cmd, t_minishell *shell);
char		*find_cmd_path(char **argv, char **envp);
char		*find_absolute_path(char **argv);
char		*find_relative_path(char *cmd, char **envp);
int			cmd_fork(t_cmd *cmd, t_minishell *shell, char *cmd_path);

/* Executor – redirections and process helpers */
int			handle_redirects(t_cmd *cmd);
int			setup_parent_redirects(t_cmd *cmd, int *saved_in, int *saved_out);
int			restore_parent_redirects(int saved_in, int saved_out, int result);
int			wait_for_child(pid_t pid);

/* Pipeline execution */
void		execute_pipeline(t_minishell *shell, t_cmd *cmds);
int			child_redirects(t_cmd *cmd, t_minishell *shell, int result);
pid_t		fork_and_execute_child(t_minishell *shell, t_cmd *current,
				int prev_fd, int *fd);
int			wait_all_children(t_minishell *shell, pid_t *child_pids, int count);
void		execute_child(t_minishell *shell, t_cmd *current,
				int prev_fd, int *fd);

/* ************************************************************************** */
/*                                  BUILTINS                                  */
/* ************************************************************************** */

/* Builtins – cd */
int			builtin_cd(t_cmd *cmd, t_minishell *shell);
int			new_old_pwd(t_cmd *cmd, t_minishell *shell);
char		*change_dir(t_cmd *cmd, t_minishell *shell, char *old_pwd);
void		set_pwd_env(t_cmd *cmd, t_minishell *shell,
				char *n_p, char *o_p);

/* Builtins – echo */
int			builtin_echo(t_cmd *cmd);
int			skip_nl(t_cmd *cmd, int i);
void		write_echo(t_cmd *cmd, int i);

/* Builtins – env */
int			builtin_env(t_minishell *shell);

/* Builtins – exit */
int			builtin_exit(t_cmd *cmd, t_minishell *shell);

/* Builtins – export */
int			builtin_export(t_cmd *cmd, t_minishell *shell);
void		write_export(t_minishell *shell);

/* Builtins – pwd */
int			builtin_pwd(void);

/* Builtins – unset */
int			builtin_unset(t_cmd *cmd, t_minishell *shell);

/* Builtins – helpers */
int			execute_builtin(t_cmd *cmd, t_minishell *shell);
int			run_builtin(t_cmd *cmd, t_minishell *shell);
int			is_builtin(t_cmd *cmd);
int			is_parent_builtin(t_cmd *cmd);
int			run_parent_builtin(t_cmd *cmd, t_minishell *shell);
int			run_child_builtin(t_cmd *cmd, t_minishell *shell);
void		copy_envp(t_minishell *shell, char **envp);
void		add_or_update_exp(t_minishell *shell, char *var);
void		add_or_update_env(t_minishell *shell, char *var);

/* ************************************************************************** */
/*                           PARSING – TOKENIZATION                           */
/* ************************************************************************** */
char		**tokenize_input(char *input);
int			skip_spaces(char *input, int i);
int			skip_special_operator(char *input, int i);
int			skip_quoted_input(char *input, int i);

/* Parsing – command structure */
t_cmd		*cmd_into_new_node(void);
char		**add_argument(char **args, char *arg);
t_cmd		*tokens_into_cmds(char **tokens, t_minishell *shell);

/* Parsing – token handling */
int			handle_pipe_token(t_parsing *p);
int			handle_redirect_token(t_parsing *p);
int			handle_heredoc_wrapper(t_parsing *p);
int			handle_heredoc_token(t_cmd *current, char *raw_token,
				int heredoc_index, t_minishell *shell);

/* ************************************************************************** */
/*                                  HEREDOC                                   */
/* ************************************************************************** */
void		handle_heredoc(const char *delimiter, char *tmpfile,
				t_minishell *shell, int expand);
char		*process_heredoc(const char *delimiter, int i,
				t_minishell *shell, int expand);
void		cleanup_heredoc_files(t_redirect *redirects);
char		*make_tmp_heredoc_filename(int i);

/* ************************************************************************** */
/*                                 EXPANSION                                  */
/* ************************************************************************** */
char		*variable_expansion(const char *input, t_minishell *shell);
char		*expand_exit_code(char *result, int exit_code);
char		*expand_variable(char const *input, int *i,
				char **envp, char *result);
char		*expand_normal_char(char const *input, int *i, char *result);
void		update_quotes(char c, int *in_single, int *in_double);

/* ************************************************************************** */
/*                               QUOTE REMOVAL                                */
/* ************************************************************************** */
char		*remove_quotes(char const *token);
char		*process_token(char *token, t_minishell *shell);

/* ************************************************************************** */
/*                                 ERROR PRINT                                */
/* ************************************************************************** */
void		print_syntax_error(t_minishell *shell, char const *token);
void		print_error(t_minishell *shell, char const *message);
void		print_error_filename(char const *filename, char const *message);

/* ************************************************************************** */
/*                         PARSING – HELPERS                                  */
/* ************************************************************************** */
int			is_space(char c);
int			is_whitespace_only(char *str);
int			is_special_op(char c);
int			count_redirects(t_redirect *list);
t_redirect	*add_redirect(t_redirect *list, char *filename,
				t_redirect_type type);

/* ************************************************************************** */
/*                                 SHELL LOOP                                 */
/* ************************************************************************** */
void		run_minishell_loop(t_minishell *shell);
void		execute_and_free(t_minishell *shell, char **tokens, char *input);
void		shell_init(t_minishell *shell, char **envp);
void		cleanup_all(t_minishell *shell);
int			check_eof_or_signal(t_minishell *shell, char **input);
void		handle_empty_input(t_minishell *shell, char *input);
char		**process_input_line(t_minishell *shell, char *input);

/* ************************************************************************** */
/*                                  HELPERS                                   */
/* ************************************************************************** */
char		*ft_strjoin_and_free(char *s1, char *s2);
char		*get_env_value(char const *name, char **envp);
size_t		count_env(char **envp);

#endif
