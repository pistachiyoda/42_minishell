#ifndef PARSER_H
# define PARSER_H
# include "libft.h"

// parser/parser.c
t_redirects	*set_redirect_member(t_list **words);
void		set_args(t_list **words, t_cmd_block *cmd);
void		set_redirects(t_list **words, t_cmd_block *cmd, t_list **redirects);
void		set_tokens(t_list **tokens, t_cmd_block *cmd);
t_list		*parser(t_list *words);

// parser/is_valid_words.c
bool		is_valid_words(t_list *words);

#endif
