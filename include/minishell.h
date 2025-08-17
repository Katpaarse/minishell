/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:27:40 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/17 14:11:22 by jukerste         ###   ########.fr       */
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
int		builtin_cd(void);
int		builtin_pwd(void);
int		builtin_echo(void);
int		builtin_env(void);
int		builtin_export(void);
int		builtin_unset(void);
int		builtin_exit(void);

//parsing functions
char	**tokenize_input(char *input);
t_cmd	*cmd_into_new_node(void);
char	**add_argument(char **args, char *arg);
t_cmd	*tokens_into_cmds(char **tokens);

//debug parsing functions
void	print_debug_cmds(t_cmd *cmds);

# endif
