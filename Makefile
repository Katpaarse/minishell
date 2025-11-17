NAME        = minishell

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g -Iinclude -O0

SRC_DIR     = src
LIBFT_DIR   = $(SRC_DIR)/libft
OBJ_DIR     = obj

LIBFT_A     = $(LIBFT_DIR)/libft.a

SRC_FILES   = main.c \
			Commands/Builtins/builtin_cd.c Commands/Builtins/builtin_echo.c Commands/Builtins/builtin_env.c Commands/Builtins/builtin_exit.c Commands/Builtins/builtin_pwd.c Commands/Builtins/builtin_export.c Commands/Builtins/builtin_unset.c Commands/Builtins/builtin_utils.c \
			Commands/Externals/external_cmd.c Commands/Externals/external_utils.c \
			Execution/executor.c Execution/executor_utils.c \
			InputOutput/pipeline_exec.c InputOutput/pipeline_helpers.c InputOutput/heredoc_exec.c InputOutput/heredoc_utils.c InputOutput/heredoc_io.c InputOutput/redirects.c \
			Parsing/tokens_into_cmds.c Parsing/parser_helpers.c Parsing/process_token.c Parsing/tokenizer.c Parsing/tokenizer_utils.c Parsing/expand_variables.c \
			Utils/cmd_utils.c Utils/env_utils.c Utils/error_messages.c Utils/free.c Utils/signals.c Utils/utils.c \

SRCS        = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJS        = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

READLINE    = -lreadline

all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) $(READLINE) -o $(NAME)

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -I$(LIBFT_DIR) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re