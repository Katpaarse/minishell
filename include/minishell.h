/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:27:40 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/14 17:10:47 by jukerste         ###   ########.fr       */
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

# define SUCCES 0
# define FAILURE 1

# define TRUE 1
# define FALSE 0

typedef struct s_cmd
{
	char			**argv; //command and arguments
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

# endif
