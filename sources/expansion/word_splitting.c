#include "minishell.h"

int	split_by_space_expand(char *str, t_list **words, int *i, int start)
{
	t_list	*new;

	if (*words == NULL)
		*words = xlstnew(xsubstr(str, start, *i - start, "expansion"),
				"expansion");
	else
	{
		new = xlstnew(xsubstr(str, start, *i - start, "expansion"),
				"expansion");
		ft_lstadd_back(words, new);
	}
	while (str[*i + 1] != '\0' && is_space_tab_newline(str[*i + 1]))
		(*i)++;
	start = *i + 1;
	return (start);
}

void	get_new_head(char **head, int j, int start)
{
	char	*tmp;

	if (start != j)
		tmp = xsubstr(*head, start, j - start, "expansion");
	else
		tmp = ft_strdup("");
	free(*head);
	*head = tmp;
}

void	word_splitting(t_list **words, int status, char **head, bool *splitted)
{
	int		j;
	int		start;

	j = 0;
	if (status == NONE)
	{
		if (!is_character_contained(*head, &j))
		{
			free(*head);
			*head = ft_strdup("");
			return ;
		}
		start = j;
		while ((*head)[j] != '\0')
		{
			if (is_space_tab_newline((*head)[j]))
			{
				start = split_by_space_expand(*head, words, &j, start);
				*splitted = true;
			}
			j++;
		}
		get_new_head(head, j, start);
	}
}
