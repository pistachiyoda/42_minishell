
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
	sources/exec_command_line/handle_heredoc.c\
	sources/exec_command_line/handle_heredoc2.c\
	sources/exec_command_line/close_doc_pipe_fds.c\
	sources/exec_command_line/handle_redirect.c\
	sources/exec_command_line/handle_input_redirect.c\
	sources/exec_command_line/handle_pipe.c\
	sources/tmp_parser/tmp_parse_data.c\
	sources/runner/run_builtin_command.c\
	sources/utils/free.c\
	sources/utils/is_env_registered.c\
	sources/utils/is_quote_type_switched.c\
	sources/utils/print_error.c\
	sources/utils/malloc_check.c\
	sources/utils/malloc_check2.c\
	sources/utils/wrapper.c\
	sources/utils/wrapper2.c\
	sources/utils/validation.c\
	sources/debug/debug_funcs.c\
	sources/debug/set_dataset.c\
	sources/env/create_environ.c\
	sources/env/t_environ_to_vector.c\
	sources/builtin/env.c\
	sources/builtin/export.c\
	sources/builtin/export_display_env.c\
	sources/builtin/unset.c\
	sources/lexer/lexer.c\
	sources/lexer/split_by_redirect_pipe.c\
	sources/parser/parser.c\
	sources/parser/set_cmd_block.c\
	sources/expansion/expansion.c\
	sources/expansion/set_expanded_to_words.c\
	sources/expansion/param_expansion.c\
	sources/expansion/word_splitting.c\
	sources/expansion/quote_removal.c
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
