
NAME = minishell
CC = gcc
INCLUDES = -I./includes/ -I./libft/
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)
LIBS = -L./libft/ -lft -lreadline
LIBFT = ./libft/libft.a
CFILES =\
	sources/main.c
OBJ = $(CFILES:.c=.o)

all: $(NAME)
$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS) 
$(LIBFT):
	make -C ./libft/
clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean all

.PHONY: all clean fclean re