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

// lexer/add_last_str.c
void	add_last_str(char *str, t_list *words, int i, int start);

// lexer/lexer.c
bool	is_space_tab_newline(char c);
int		is_in_quote_dquote(char *str, int i, int status);
int		split_by_space(char *str, t_list *words, int *i, int start);
int		split_by_redirect_pipe(char *str, t_list *words, int *i, int start);
void	lexer(char *str);

#endif
