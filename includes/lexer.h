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
bool	get_valid_fd_num(char *str, t_list **words, int i, int start);
bool	is_valid_redirect_pipe(char *str, t_list *words, int *i, int start);
int		split_by_redirect_pipe(char *str, t_list *words, int *i, int start);

// lexer/lexer.c
bool	is_space_tab_newline(char c);
int		split_by_space_lex(char *str, t_list *words, int *i, int start);
bool	add_last_str(char *str, t_list *words, int start, int status);
t_list	*lexer(char *str);

#endif
