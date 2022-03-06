#include "minishell.h"

int	split_by_space2(char *str, t_list **words, int *i, int start)
{
	t_list	*new;

	if (*words == NULL)
		*words = xlstnew(xsubstr(str, start, *i - start, "lexer"), "lexer");
	else
	{
		new = xlstnew(xsubstr(str, start, *i - start, "lexer"), "lexer");
		ft_lstadd_back(words, new);
	}
	while (str[*i + 1] != '\0' && is_space_tab_newline(str[*i + 1]))
		(*i)++;
	start = *i + 1;
	return (start);
}

char	*word_split(t_list **words, int status, char *head, int *splitted)
{
	int		j;
	int		start;
	char	*tmp;

	j = 0;
	start = 0;
	if (status == NONE)
	{
		while (ft_strlen(head) - j > 0)
		{
			if (is_space_tab_newline(head[j]))
			{
				start = split_by_space2(head, words, &j, start);
				*splitted = 1;
			}
			j++;
		}
		if (start != j)
		{
			tmp = xsubstr(head, start, j - start, "expansion");
			free(head);
			head = tmp;
		}
	}
	return (head);
}
