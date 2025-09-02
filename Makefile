NAME        = minishell

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g -Iinclude

SRC_DIR     = src
LIBFT_DIR   = $(SRC_DIR)/libft
OBJ_DIR     = obj

LIBFT_A     = $(LIBFT_DIR)/libft.a

SRC_FILES   = builtin_cd.c builtin_echo.c builtin_env.c builtin_exit.c builtin_pwd.c builtin_unset.c builtin_utils.c \
			cmd_utils.c env_utils.c expand_variables.c main.c parser.c process_token.c redirects.c signals.c tokenizer.c utils.c error_message.c
SRCS        = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJS        = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

READLINE    = -lreadline

all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) $(READLINE) -o $(NAME)

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -I$(LIBFT_DIR) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re