
NAME = minishell
CC = gcc
INCLUDES = -I./includes/ -I./libft/
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)
LIBS = -L./libft/ -lft -lreadline
LIBFT = ./libft/libft.a
CFILES =\
	sources/main.c\
	sources/exec_command/exec_command.c\
	sources/exec_command/resolve_env.c\
	sources/exec_command/utils.c\
	sources/exec_command_line/exec_command_line.c\
	sources/exec_command_line/handle_file.c\
	sources/tmp_parser/tmp_parse_data.c\
	sources/runner/run_builtin_command.c\
	sources/utils/free.c\
	sources/utils/print_error.c\
	sources/utils/malloc_check.c\
	sources/debug/debug_funcs.c\
	sources/env/create_environ.c\
	sources/env/t_environ_to_vector.c\
	sources/builtin/env.c\
	sources/builtin/export.c\
	sources/builtin/export_display_env.c\
	sources/builtin/unset.c\
	sources/lexer/lexer.c\
	sources/lexer/add_last_str.c
OBJ = $(CFILES:.c=.o)

all: $(NAME)
$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS)
$(LIBFT):
	make -C ./libft bonus
clean:
	rm -f $(OBJ)
	make -C ./libft clean
fclean: clean
	rm -f $(NAME)
	make -C ./libft fclean
re: fclean all

.PHONY: all clean fclean re
