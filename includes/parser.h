#ifndef PARSER_H
# define PARSER_H
# include "libft.h"

// parser/parser.c
bool		is_valid_words(t_list *words);
bool		is_redirect(t_list *words);
void		set_tokens(t_list **tokens, t_cmd_block *cmd);
t_list		*parser(t_list *words);

// parser/set_cmd_block.c
t_redirects	*set_redirect_member(t_list **words);
void		set_first_args(t_list **words, t_cmd_block *cmd, int arg_num);
void		add_args(t_list **words, t_cmd_block *cmd, int arg_num);
void		set_args(t_list **words, t_cmd_block *cmd);
void		set_cmd_block(t_list **words, t_cmd_block *cmd);

#endif
