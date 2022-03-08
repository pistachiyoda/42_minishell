#ifndef EXPANSION_H
# define EXPANSION_H
# include "libft.h"

typedef struct s_quote
{
	int	prev_q;
	int	status;
	int	managed_i;
}	t_quote;

// sources/expansion/expansion.c
void	assign_expanded_cmd_args(t_cmd_block *cmd, t_list *words);
void	expand_cmd_args(t_cmd_block *cmd, t_environ *env, t_list *words);
void	assign_expanded_target(t_cmd_block *cmd, t_list *words, bool error);
void	expand_redirects(t_cmd_block *cmd, t_environ *env, t_list *words);
void	expansion(t_list **tokens, t_environ *env);

// sources/expansion/set_expanded_to_words.c
char	*left_to_next_head(char *left, char *head, int status);
void	add_to_words(t_list **words, char *head, char *str);
bool	set_expanded_to_words(t_environ *env, char *str, t_list **words);

// sources/expansion/param_expansion.c
size_t	get_left_len(char *str, int i);
int		param_expansion(t_environ *env, char *str, char **head, int *i);

// sources/expansion/word_splitting.c
int		split_by_space_expand(char *str, t_list **words, int *i, int start);
void	word_splitting(t_list **words, int status, char **head, bool *splitted);

// sources/expansion/quote_removal.c
void	concat_normal_str(char *str, char **head, int i, t_quote *quote);
void	concat_expanded_and_left(char *str, char **head, int i, t_quote *quote);
void	concat_all(char *str, char **head, int i);
void	quote_removal(char *str, char **head, int i, t_quote *quote);
void	set_head_before_dollar(char *str, char **head, int i, int prev_q);

#endif
