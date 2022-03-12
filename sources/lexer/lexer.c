#include "minishell.h"

int	split_by_space_lex(char *str, t_list *words, int *i, int start)
{
	t_list	*new;

	new = ft_xlstnew(ft_xsubstr(str, start, *i - start, "lex"), "lex");
	ft_lstadd_back(&words, new);
	while (str[*i + 1] != '\0' && is_space_tab_newline(str[*i + 1]))
		(*i)++;
	start = *i + 1;
	return (start);
}

bool	add_last_str(char *str, t_list **words, int start, int status)
{
	t_list	*next_ele;
	t_list	*last;
	int		i;

	last = ft_lstlast(*words);
	i = ft_strlen(str);
	if ((start == i && (ft_strncmp(last->content, ">", 1) == 0
				|| ft_strncmp(last->content, "<", 1) == 0
				|| ft_strncmp(last->content, ">>", 2) == 0
				|| ft_strncmp(last->content, "<<", 2) == 0)) || status != NONE)
	{
		syntax_error("newline");
		return (false);
	}
	if (start < i)
		ft_lstadd_back(words,
			ft_xlstnew(ft_xsubstr(str, start, i - start, "lex"), "lex"));
	next_ele = (*words)->next;
	free(*words);
	*words = next_ele;
	return (true);
}

bool	lexer(char *str, t_list **words)
{
	int		i;
	int		start;
	int		status;

	i = 0;
	status = NONE;
	*words = ft_xlstnew(NULL, "lex");
	if (!is_character_contained(str, &i))
		return (free_words_str(words, str));
	start = i;
	while (str[i] != '\0' && start != -1)
	{
		is_quote_type_switched(str, i, &status);
		if (status == NONE && is_space_tab_newline(str[i]))
			start = split_by_space_lex(str, *words, &i, start);
		else if (status == NONE
			&& (str[i] == '>' || str[i] == '<' || str[i] == '|'))
			start = split_by_redirect_pipe(str, *words, &i, start);
		i++;
	}
	if (start != -1 && add_last_str(str, words, start, status))
		return (true);
	return (free_words_str(words, str));
}
