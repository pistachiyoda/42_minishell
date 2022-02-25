#include "minishell.h"

bool	is_valid_redirect_pipe(t_list *words, char *str, int i)
{
	t_list	*last;

	last = ft_lstlast(words);
	if (last->content == NULL)
		return (true);
	if ((str[i] == '|' && ((char *)last->content)[0] == '|')
		|| (((char *)last->content)[0] == '>'
		|| ((char *)last->content)[0] == '<'))
	{
		str[i + 1] = '\0';
		syntax_error(&str[i]);
		ft_lstclear(&words, free);
		free(str);
		return (false);
	}
	return (true);
}

void	add_fd_num(char *str, t_list **words, int i, int start)
{
	t_list	*new;
	int		count;

	new = NULL;
	count = 0;
	while (i - start > count)
	{
		if (ft_isdigit(str[start + count]) != 1)
			break ;
		count++;
	}
	if ((count == 0 || i - start != count) && str[i] != '|')
	{
		if (str[i] == '>')
			new = xlstnew(xstrdup("1", "lexer"), "lexer");
		else if (str[i] == '<')
			new = xlstnew(xstrdup("0", "lexer"), "lexer");
		ft_lstadd_back(words, new);
	}
}

int	split_by_redirect_pipe(char *str, t_list *words, int *i, int start)
{
	t_list	*new;

	if (*i != start)
	{
		new = xlstnew(xsubstr(str, start, *i - start, "lexer"), "lexer");
		ft_lstadd_back(&words, new);
	}
	if (!is_valid_redirect_pipe(words, str, *i))
		return (-1);
	add_fd_num(str, &words, *i, start);
	start = *i;
	if (ft_strncmp(&str[*i], ">>", 2) == 0
		|| ft_strncmp(&str[*i], "<<", 2) == 0)
	{
		(*i)++;
		new = xlstnew(xsubstr(str, start, 2, "lexer"), "lexer");
	}
	else
		new = xlstnew(xsubstr(str, start, 1, "lexer"), "lexer");
	ft_lstadd_back(&words, new);
	while (str[*i + 1] != '\0' && is_space_tab_newline(str[*i + 1]))
		(*i)++;
	return (start = *i + 1);
}
