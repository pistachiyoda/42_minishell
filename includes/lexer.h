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

// lexer/split_by_redirect_pipe.c
bool	is_valid_redirect_pipe(t_list *words, char *str, int i);
void	add_fd_num(char *str, t_list **words, int i, int start);
int		split_by_redirect_pipe(char *str, t_list *words, int *i, int start);

// lexer/lexer.c
bool	is_space_tab_newline(char c);
int		is_in_quote_dquote(char *str, int i, int status);
int		split_by_space(char *str, t_list *words, int *i, int start);
bool	add_last_str(char *str, t_list *words, int i, int start);
t_list	*lexer(char *str);

#endif
