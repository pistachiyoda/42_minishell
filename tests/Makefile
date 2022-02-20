
NAME = minishell_test
CC = gcc
INCLUDES = -I../includes/ -I../libft/
CXXFLAGS=-D_GLIBCXX_DEBUG -std=c++11 -Wall -Wextra -g -I /opt/homebrew/Cellar/cpputest/4.0/include -L /opt/homebrew/Cellar/cpputest/4.0/lib
LDLIBS=-lCppUTest
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)
LIBS = -L../libft/ -lft -lreadline
LIBFT = ../libft/libft.a
SOURCES =\
	../sources/exec_command/exec_command.c\
	../sources/exec_command/resolve_env.c\
	../sources/exec_command/utils.c\
	../sources/exec_command_line/exec_command_line.c\
	../sources/exec_command_line/handle_file.c\
	../sources/tmp_parser/tmp_parse_data.c\
	../sources/runner/run_builtin_command.c\
	../sources/utils/free.c\
	../sources/utils/print_error.c\
	../sources/utils/malloc_check.c\
	../sources/debug/debug_funcs.c\
	../sources/env/create_environ.c\
    ../sources/env/t_environ_to_vector.c\
    ../sources/builtin/env.c\
    ../sources/builtin/export.c\
	../sources/builtin/export_display_env.c\
    ../sources/builtin/unset.c\
	../sources/lexer/lexer.c\
	../sources/lexer/add_last_str.c
OBJ = $(SOURCES:.c=.o)

all: $(NAME)
$(NAME): exec_cmdline.o lexer.o $(OBJ) $(LIBFT)
	c++ $(CXXFLAGS) $(INCLUDES) exec_cmdline.o lexer.o $(OBJ) -o $(NAME) $(LIBS) $(LDLIBS)
exec_cmdline.o: ./exec_cmdline/exec_cmdline.cpp
	c++ $(CXXFLAGS) $(INCLUDES) -c ./exec_cmdline/exec_cmdline.cpp $(LIBS)
lexer.o: ./lexer/lexer.cpp
	c++ $(CXXFLAGS) $(INCLUDES) -c ./lexer/lexer.cpp $(LIBS)
$(LIBFT):
	make -C ../libft bonus
clean:
	rm -f $(OBJ)
	make -C ../libft clean
fclean: clean
	rm -f $(NAME)
	make -C ..//libft fclean
re: fclean all

.PHONY: all clean fclean re
