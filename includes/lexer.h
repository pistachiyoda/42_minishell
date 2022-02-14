#ifndef LEXER_H
# define LEXER_H
# include <stdbool.h>
# include "libft.h"

# define QUOAT 39
# define DQUOAT 34

typedef struct s_list{
	char	*content;
	t_list	*next;
}				t_list;

void	lexer(char *str);

#endif
