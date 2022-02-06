
NAME = minishell
CC = gcc
INCLUDES = -I./includes/ -I./libft/
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)
LIBS = -L./libft/ -lft -lreadline
LIBFT = ./libft/libft.a
CFILES =\
	sources/main.c\
	sources/parser/parse_data.c\
	sources/runner/run_command.c
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
