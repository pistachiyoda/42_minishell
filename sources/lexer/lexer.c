#include "minishell.h"

bool	is_space_tab_newline(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (true);
	return (false);
}

int	is_in_quote_dquote(char *str, int i, int status)
{
	if (str[i] == '\'' && status != DQUOTE)
	{
		if (status == NONE)
			status = QUOTE;
		else if (status == QUOTE)
			status = NONE;
	}
	else if (str[i] == '"' && status != QUOTE)
	{
		if (status == NONE)
			status = DQUOTE;
		else if (status == DQUOTE)
			status = NONE;
	}
	return (status);
}

int	split_by_space_lex(char *str, t_list *words, int *i, int start)
{
	t_list	*new;

	new = xlstnew(xsubstr(str, start, *i - start, "lexer"), "lexer");
	ft_lstadd_back(&words, new);
	while (str[*i + 1] != '\0' && is_space_tab_newline(str[*i + 1]))
		(*i)++;
	start = *i + 1;
	return (start);
}

bool	add_last_str(char *str, t_list *words, int start, int status)
{
	t_list	*new_ele;
	t_list	*last;
	int		i;

	last = ft_lstlast(words);
	i = ft_strlen(str);
	if ((start == i && (ft_strncmp(last->content, ">", 1) == 0
				|| ft_strncmp(last->content, "<", 1) == 0
				|| ft_strncmp(last->content, ">>", 2) == 0
				|| ft_strncmp(last->content, "<<", 2) == 0)) || status != NONE)
	{
		syntax_error("newline");
		ft_lstclear(&words, free);
		free(str);
		return (false);
	}
	if (start < i)
	{
		new_ele = xlstnew(xsubstr(str, start, i - start, "lexer"), "lexer");
		ft_lstadd_back(&words, new_ele);
		return (true);
	}
	return (true);
}

t_list	*lexer(char *str)
{
	t_list	*words;
	int		i;
	int		start;
	int		status;

	i = 0;
	status = NONE;
	words = xlstnew(NULL, "lexer");
	while (str[i] != '\0' && is_space_tab_newline(str[i]))
		i++;
	start = i;
	while (str[i] != '\0' && start != -1)
	{
		status = is_in_quote_dquote(str, i, status);
		if (status == NONE && is_space_tab_newline(str[i]))
			start = split_by_space_lex(str, words, &i, start);
		else if (status == NONE
			&& (str[i] == '>' || str[i] == '<' || str[i] == '|'))
			start = split_by_redirect_pipe(str, words, &i, start);
		i++;
	}
	if (start != -1 && add_last_str(str, words, start, status))
		return (words);
	return (NULL);
}
