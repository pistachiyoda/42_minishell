
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
	../sources/tmp_parser/tmp_parse_data.c\
	../sources/runner/run_builtin_command.c\
	../sources/utils/free.c\
	../sources/utils/print_error.c\
	../sources/debug/debug_funcs.c\
	../sources/env/create_environ.c\
    ../sources/env/t_environ_to_vector.c\
    ../sources/builtin/env.c\
    ../sources/builtin/export.c\
    ../sources/builtin/unset.c
OBJ = $(SOURCES:.c=.o)

all: $(NAME)
$(NAME): minishell_test.o $(OBJ) $(LIBFT)
	c++ $(CXXFLAGS) $(INCLUDES) minishell_test.o $(OBJ) -o $(NAME) $(LIBS) $(LDLIBS)
minishell_test.o: ./exec_cmdline/minishell_test.cpp
	c++ $(CXXFLAGS) $(INCLUDES) -c ./exec_cmdline/minishell_test.cpp $(LIBS)
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
