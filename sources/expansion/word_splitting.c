#include "minishell.h"

bool	is_space_condition(t_expand data, char **head, bool *split, int *j)
{
	if (check_str_type(*head) != SPACELESS)
		*split = true;
	if (check_str_type(*head) == ALL_SPACE)
	{
		free(*head);
		if (!data.end)
			*head = ft_xstrdup("", "expansion");
		return (false);
	}
	while ((*head)[*j] != '\0' && is_space_tab_newline((*head)[*j]))
		(*j)++;
	return (true);
}

void	split_by_space_expand(char **str, t_list **words, int *i, int start)
{
	t_list	*new;

	if (*words == NULL)
		*words = ft_xlstnew(ft_xsubstr(*str, start, *i - start, "expansion"),
				"expansion");
	else
	{
		new = ft_xlstnew(ft_xsubstr(*str, start, *i - start, "expansion"),
				"expansion");
		ft_lstadd_back(words, new);
	}
}

int	skip_blank(char *str, int *i)
{
	while (str[*i + 1] != '\0' && is_space_tab_newline(str[*i + 1]))
		(*i)++;
	return (*i);
}

void	get_new_head(char **head, int j, int start)
{
	char	*tmp;

	if (start != j)
		tmp = ft_xsubstr(*head, start, j - start, "expansion");
	else
		tmp = ft_xstrdup("", "expansion");
	free(*head);
	*head = tmp;
}

void	word_splitting(t_list **words, t_expand data, char **head, bool *split)
{
	int		j;
	int		start;

	j = 0;
	if (data.status == NONE)
	{
		if (!is_space_condition(data, head, split, &j))
			return ;
		start = j;
		while ((*head)[j] != '\0')
		{
			if (is_space_tab_newline((*head)[j]))
			{
				split_by_space_expand(head, words, &j, start);
				if (data.end && check_str_type(*head) == ALL_SPACE)
				{
					free(*head);
					return ;
				}
				start = skip_blank(*head, &j) + 1;
			}
			j++;
		}
		get_new_head(head, j, start);
	}
}
