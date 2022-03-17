#include "minishell.h"

bool	get_valid_fd_num(char *str, t_list *words, int i, int start)
{
	int		count;
	char	*fd;

	count = 0;
	while (i - start > count && ft_isdigit(str[start + count]) == 1)
		count++;
	fd = ft_xsubstr(str, start, count, "lex");
	if (i - start == count && (count > 10 || ft_atoi(fd) == -1))
	{
		print_error("file descriptor out of range", EMESS_LARGE_FD);
		free(fd);
		return (false);
	}
	if ((count == 0 || i - start != count) && str[i] != '|')
	{
		if (str[i] == '>')
			ft_lstadd_back(&words, ft_xlstnew(ft_xstrdup("1", "lex"), "lex"));
		else if (str[i] == '<')
			ft_lstadd_back(&words, ft_xlstnew(ft_xstrdup("0", "lex"), "lex"));
	}
	free(fd);
	return (true);
}

bool	is_valid_redirect_pipe(char *str, int *i, int start)
{
	int		j;

	j = 1;
	while (str[*i + 1] != '\0' && is_blank(str[*i + 1]))
		(*i)++;
	while (ft_isdigit(str[*i + j]) == 1)
		j++;
	if ((str[start] == '|' && str[*i + 1] == '|') || (str[start] != '|'
			&& (str[*i + 1] == '<' || str[*i + 1] == '>' || str[*i + 1] == '|'))
		|| (j != 1 && (str[*i + j] == '<' || str[*i + j] == '>')))
	{
		if (j != 1)
		{
			str[*i + j] = '\0';
			if (ft_atoi(&str[*i + 1]) == -1)
				ft_strlcpy(&str[*i + 1], "-1", 3);
		}
		else
			str[*i + j + 1] = '\0';
		syntax_error(&str[*i + 1]);
		return (false);
	}
	return (true);
}

int	split_by_redirect_pipe(char *str, t_list *words, int *i, int start)
{
	t_list	*new;

	if (*i != start)
	{
		new = ft_xlstnew(ft_xsubstr(str, start, *i - start, "lex"), "lex");
		ft_lstadd_back(&words, new);
	}
	if (!get_valid_fd_num(str, words, *i, start))
		return (-1);
	start = *i;
	if (ft_strncmp(&str[*i], ">>", 2) == 0
		|| ft_strncmp(&str[*i], "<<", 2) == 0)
	{
		(*i)++;
		new = ft_xlstnew(ft_xsubstr(str, start, 2, "lex"), "lex");
	}
	else
		new = ft_xlstnew(ft_xsubstr(str, start, 1, "lex"), "lex");
	ft_lstadd_back(&words, new);
	if (!is_valid_redirect_pipe(str, i, start))
		return (-1);
	return (start = *i + 1);
}
