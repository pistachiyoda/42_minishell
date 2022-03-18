#ifndef LEXER_H
# define LEXER_H
# include <stdbool.h>
# include "libft.h"

enum e_STATUS
{
	QUOTE,
	DQUOTE,
	NONE
};

typedef struct s_lex
{
	int		i;
	int		start;
	int		status;
	bool	error;
}	t_lex;

// lexer/split_by_redirect_pipe.c
bool	split_by_redirect_pipe(char *str, t_list *words, t_lex *info);

// lexer/lexer.c
void	split_by_space_lex(char *str, t_list *words, t_lex *info);
bool	add_last_str(char *str, t_list **words, t_lex info);
bool	lexer(char *str, t_list **words);

#endif
