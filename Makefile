NAME        = minishell

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g -Iinclude

SRC_DIR     = src
LIBFT_DIR   = $(SRC_DIR)/libft
OBJ_DIR     = obj

LIBFT_A     = $(LIBFT_DIR)/libft.a

SRC_FILES   = Builtins/builtin_cd.c Builtins/builtin_echo.c Builtins/builtin_env.c Builtins/builtin_exit.c Builtins/builtin_pwd.c Builtins/builtin_export.c Builtins/builtin_unset.c Builtins/builtin_utils.c \
			cmd_utils.c env_utils.c executor.c expand_variables.c main.c parser.c process_token.c redirects.c signals.c tokenizer.c utils.c error_message.c Externals/external_cmd.c Externals/external_utils.c
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