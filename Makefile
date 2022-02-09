
NAME = minishell
CC = gcc
INCLUDES = -I./includes/ -I./libft/ -I$(shell brew --prefix readline)/include
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)
LIBS = -L./libft/ -lft -lreadline -lhistory -L$(shell brew --prefix readline)/lib
LIBFT = ./libft/libft.a
CFILES =\
	sources/main.c\
	sources/exec_command/exec_command.c\
	sources/exec_command/resolve_env.c\
	sources/exec_command/utils.c\
	sources/utils/free.c\
	sources/utils/print_error.c\
	sources/signal.c
OBJ = $(CFILES:.c=.o)

all: $(NAME)
$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS)
$(LIBFT):
	make -C ./libft/
clean:
	rm -f $(OBJ)
	make -C ./libft clean
fclean: clean
	rm -f $(NAME)
	make -C ./libft fclean
re: fclean all

.PHONY: all clean fclean re
