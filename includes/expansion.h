#ifndef EXPANSION_H
# define EXPANSION_H
# include "libft.h"

typedef struct s_expand
{
	int		prev_q;
	int		status;
	int		i;
	int		managed_i;
	int		h_len;
	bool	end;
	bool	rdr_error;
}	t_expand;

enum e_SPACE
{
	SPACELESS,
	PARTIAL_SPACE,
	ALL_SPACE
};

// sources/expansion/expansion.c
void		assign_expanded_cmd_args(t_cmd_block *cmd, t_list **words);
void		expand_cmd_args(t_cmd_block *cmd, t_environ *env);
void		assign_expanded_target(t_redirects *redir, t_list **words);
void		expand_redirects(t_cmd_block *cmd, t_environ *env);
void		expansion(t_list **tokens, t_environ *env);

// sources/expansion/set_expanded_to_words.c
void		add_to_words(t_list **words, char *head, char *str);
t_expand	init_struct(void);
bool		set_expanded_to_words(
				t_environ *env, char *str, t_list **words, int type);

// sources/expansion/param_expansion.c
size_t		get_left_len(char *str, int i);
void		expand_exit_status(t_expand *data, char **head);
void		add_dollar_mark(char **head, char *str);
char		*expand_env(t_environ *env, t_expand *data, char *str, char **head);
void		param_expansion(
				t_environ *env, t_expand *data, char *str, char **head);

// sources/expansion/word_splitting.c
bool		is_allspace_or_null(t_expand *data, char **head, int *j);
void		split_by_space_expand(char **str, t_list **words, int i, int start);
void		get_new_head(char **head, int j, int start);
int			split_before_expanded(
				t_list **words, t_expand *data, char **head, int j);
void		word_splitting(t_list **words, t_expand *data, char **head);

// sources/expansion/quote_removal.c
void		concat_normal_str(char *str, char **head, t_expand *data);
void		concat_expanded_and_left(char *str, char **head, t_expand *data);
void		concat_all(char *str, char **head, int i);
void		quote_removal(char *str, char **head, t_expand *data, int type);
void		set_head_before_dollar(char *str, char **head, t_expand data);

#endif
