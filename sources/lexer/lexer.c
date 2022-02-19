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

int	split_by_space(char *str, t_list *words, int *i, int start)
{
	t_list	*new_ele;

	new_ele = ft_lstnew(xsubstr(str, start, *i - start, "lexer"));
	malloc_check(new_ele, "lexer");
	ft_lstadd_back(&words, new_ele);
	while (str[*i + 1] != '\0' && is_space_tab_newline(str[*i + 1]))
		(*i)++;
	start = *i + 1;
	return (start);
}

int	split_by_redirect_pipe(char *str, t_list *words, int *i, int start)
{
	t_list	*new_ele;

	if (is_space_tab_newline(str[*i - 1]) == false && str[*i - 1] != '|'
		&& str[*i - 1] != '>' && str[*i - 1] != '<')
	{
		new_ele = ft_lstnew(xsubstr(str, start, *i - start, "lexer"));
		malloc_check(new_ele, "lexer");
		ft_lstadd_back(&words, new_ele);
		start = *i;
	}
	if (ft_strncmp(&str[*i], ">>", 2) == 0
		|| ft_strncmp(&str[*i], "<<", 2) == 0)
	{
		(*i)++;
		new_ele = ft_lstnew(xsubstr(str, start, 2, "lexer"));
	}
	else
		new_ele = ft_lstnew(xsubstr(str, start, 1, "lexer"));
	malloc_check(new_ele, "lexer");
	ft_lstadd_back(&words, new_ele);
	while (str[*i + 1] != '\0' && is_space_tab_newline(str[*i + 1]))
		(*i)++;
	start = *i + 1;
	return (start);
}

bool	lexer(char *str)
{
	t_list	*words;
	int		i;
	int		start;
	int		status;

	i = 0;
	start = 0;
	status = NONE;
	words = ft_lstnew(NULL);
	malloc_check(words, "lexer");
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '"')
			status = is_in_quote_dquote(str, i, status);
		else if (status == NONE && is_space_tab_newline(str[i]))
			start = split_by_space(str, words, &i, start);
		else if (status == NONE
			&& (str[i] == '>' || str[i] == '<' || str[i] == '|'))
			start = split_by_redirect_pipe(str, words, &i, start);
		i++;
	}
	return (add_last_str(str, words, i, start));
}
