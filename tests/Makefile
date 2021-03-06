
NAME = minishell_test
CC = gcc
C_MEMOLY_LEAK = -include /usr/local/opt/cpputest/include/CppUTest/MemoryLeakDetectorMallocMacros.h
CXX_MEMOLY_LEAK = -include /usr/local/opt/cpputest/include/CppUTest/MemoryLeakDetectorNewMacros.h
INCLUDES = -I../includes/ -I../libft/ -I$(shell brew --prefix readline)/include
CXXFLAGS=-D_GLIBCXX_DEBUG -std=c++11 -Wall -Wextra -g -I /opt/homebrew/Cellar/cpputest/4.0/include -L /opt/homebrew/Cellar/cpputest/4.0/lib
LDLIBS=-lCppUTest
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)
LIBS = -L../libft/ -lft -lreadline -lhistory -L$(shell brew --prefix readline)/lib
LIBFT = ../libft/libft.a
SOURCES =\
	../sources/exec_command/exec_command.c\
	../sources/exec_command/resolve_env.c\
	../sources/exec_command/utils.c\
	../sources/exec_command_line/exec_command_line.c\
	../sources/exec_command_line/exec_command_line2.c\
	../sources/exec_command_line/handle_file.c\
	../sources/exec_command_line/handle_heredoc.c\
	../sources/exec_command_line/handle_heredoc2.c\
	../sources/exec_command_line/handle_heredoc3.c\
	../sources/exec_command_line/close_doc_pipe_fds.c\
	../sources/exec_command_line/handle_redirect.c\
	../sources/exec_command_line/handle_input_redirect.c\
	../sources/exec_command_line/handle_pipe.c\
	../sources/runner/run_builtin_command.c\
	../sources/exec_builtin/exec_builtin_only_command.c\
    ../sources/signal/signal.c\
	../sources/utils/free.c\
	../sources/utils/free2.c\
	../sources/utils/check_str_type.c\
	../sources/utils/check_char_type.c\
	../sources/utils/print_error.c\
	../sources/utils/malloc_check.c\
	../sources/utils/malloc_check2.c\
	../sources/utils/wrapper.c\
	../sources/utils/wrapper2.c\
	../sources/utils/validation.c\
	../sources/utils/ft_atol.c\
	../sources/utils/get_env_val.c\
	../sources/utils/exit_program.c\
	../sources/env/create_environ.c\
    ../sources/env/t_environ_to_vector.c\
	../sources/env/is_env_registered.c\
    ../sources/builtin/env.c\
    ../sources/builtin/export.c\
	../sources/builtin/export_display_env.c\
    ../sources/builtin/unset.c\
	../sources/builtin/pwd.c\
	../sources/builtin/echo.c\
	../sources/builtin/exit.c\
	../sources/builtin/cd.c\
	../sources/builtin/cd2.c\
	../sources/lexer/lexer.c\
	../sources/lexer/split_by_redirect_pipe.c\
	../sources/parser/parser.c\
	../sources/parser/set_args.c\
	../sources/parser/set_redirect.c\
	../sources/expansion/expansion.c\
	../sources/expansion/set_expanded_to_words.c\
	../sources/expansion/param_expansion.c\
	../sources/expansion/expand_env.c\
	../sources/expansion/word_splitting.c\
	../sources/expansion/quote_removal.c
OBJ = $(SOURCES:.c=.o)

all: $(NAME)
$(NAME): exec_cmdline.o lexer.o parser.o builtins.o utils.o expansion.o export.o expand_env_variables_in_buf.o pwd.o $(OBJ) $(LIBFT)
	c++ $(CXXFLAGS) $(INCLUDES) exec_cmdline.o lexer.o parser.o builtins.o utils.o expansion.o export.o expand_env_variables_in_buf.o pwd.o $(OBJ) -o $(NAME) $(LIBS) $(LDLIBS)
exec_cmdline.o: ./exec_cmdline/exec_cmdline.cpp
	c++ $(CXXFLAGS) $(INCLUDES) -c ./exec_cmdline/exec_cmdline.cpp $(LIBS)
lexer.o: ./lexer/lexer.cpp
	c++ $(CXXFLAGS) $(INCLUDES) -c ./lexer/lexer.cpp $(LIBS)
parser.o: ./parser/parser.cpp
	c++ $(CXXFLAGS) $(INCLUDES) -c ./parser/parser.cpp $(LIBS)
builtins.o: ./builtins/builtins.cpp
	c++ $(CXXFLAGS) $(INCLUDES) -c ./builtins/builtins.cpp $(LIBS)
utils.o: ./utils/utils.cpp
	c++ $(CXXFLAGS) $(INCLUDES) -c ./utils/utils.cpp $(LIBS)
expansion.o: ./expansion/expansion.cpp
	c++ $(CXXFLAGS) $(INCLUDES) -c ./expansion/expansion.cpp $(LIBS)
export.o: ./builtin/export.cpp
	c++ $(CXXFLAGS) $(INCLUDES) -c ./builtin/export.cpp $(LIBS)
expand_env_variables_in_buf.o: ./expand_env/expand_env_variables_in_buf.cpp
	c++ $(CXXFLAGS) $(INCLUDES) -c ./expand_env/expand_env_variables_in_buf.cpp $(LIBS)
pwd.o: ./builtin/pwd.cpp
	c++ $(CXXFLAGS) $(INCLUDES) -c ./builtin/pwd.cpp $(LIBS)
$(LIBFT):
	make -C ../libft bonus
clean:
	rm -f $(OBJ)
	make -C ../libft clean
fclean: clean
	rm -f $(NAME) *.o
	make -C ..//libft fclean
re: fclean all

.PHONY: all clean fclean re
