#ifndef EXPANSION_H
# define EXPANSION_H
# include "libft.h"

// sources/expansion/expansion.c
void	assign_expanded_cmd_args(t_cmd_block *cmd, t_list *words);
void	expand_cmd_args(t_cmd_block *cmd, t_environ *env, t_list *words);
void	assign_expanded_target(t_cmd_block *cmd, t_list **words, int status);
void	expand_redirects(t_environ *env, t_cmd_block *cmd, t_list *words);
t_list	*expansion(t_list *tokens, t_environ *env);

// sources/expansion/set_expanded_to_words.c
char	*left_to_next_head(char *left, char *head, int status);
void	add_to_words(t_list **words, char *head, char *str);
int		set_expanded_to_words(t_environ *env, char *str, t_list **words);

// sources/expansion/param_expansion.c
size_t	get_left(char *str, int i, char **left);
char	*param_expansion(t_environ *env, char *str, char **head, int *i);

// sources/expansion/word_split.c
int		split_by_space2(char *str, t_list **words, int *i, int start);
char	*word_split(t_list **words, int status, char *head, int *splitted);

#endif
