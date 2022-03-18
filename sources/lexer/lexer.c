#include "minishell.h"

void	split_by_space_lex(char *str, t_list *words, t_lex *info)
{
	t_list	*new;

	new = ft_xlstnew(
			ft_xsubstr(str, info->start, info->i - info->start, "lex"), "lex");
	ft_lstadd_back(&words, new);
	while (str[info->i + 1] != '\0' && is_blank(str[info->i + 1]))
		info->i += 1;
	info->start = info->i + 1;
}

bool	add_last_str(char *str, t_list **words, t_lex info)
{
	t_list	*next_ele;
	t_list	*last;
	int		i;

	last = ft_lstlast(*words);
	i = ft_strlen(str);
	if ((info.start == i && (ft_strncmp(last->content, ">", 1) == 0
				|| ft_strncmp(last->content, "<", 1) == 0
				|| ft_strncmp(last->content, ">>", 2) == 0
				|| ft_strncmp(last->content, "<<", 2) == 0))
		|| info.status != NONE)
	{
		syntax_error("newline");
		return (false);
	}
	if (info.start < i)
		ft_lstadd_back(words, ft_xlstnew(
				ft_xsubstr(str, info.start, i - info.start, "lex"), "lex"));
	next_ele = (*words)->next;
	free(*words);
	*words = next_ele;
	return (true);
}

static bool	check_error(char *str, t_list **words, t_lex info)
{
	if (info.error)
		free_words_str(words, str);
	else if (!add_last_str(str, words, info))
	{
		g_status = 258;
		free_words_str(words, str);
	}
	else
		return (true);
	return (false);
}

bool	lexer(char *str, t_list **words)
{
	t_lex	info;

	info.i = 0;
	info.status = NONE;
	info.error = false;
	*words = ft_xlstnew(NULL, "lex");
	if (!is_character_contained(str, &info.i))
		return (free_words_str(words, str));
	info.start = info.i;
	while (str[info.i] != '\0' && !info.error)
	{
		is_quote_type_switched(str, info.i, &info.status);
		if (info.status == NONE && is_blank(str[info.i]))
			split_by_space_lex(str, *words, &info);
		else if (info.status == NONE
			&& (str[info.i] == '>' || str[info.i] == '<' || str[info.i] == '|'))
			split_by_redirect_pipe(str, *words, &info);
		info.i++;
	}
	return (check_error(str, words, info));
}
