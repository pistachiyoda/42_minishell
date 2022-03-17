#include "minishell.h"

bool	is_allspace_or_null(t_expand *data, char **head, int *j)
{
	int	i;

	i = 0;
	if (*head == NULL)
		return (true);
	if (check_str_type(*head) == ALL_SPACE && data->status == NONE && data->end
		&& data->h_len == 0)
	{
		free(*head);
		return (true);
	}
	*j = data->h_len;
	while ((*head)[*j] != '\0' && is_blank((*head)[*j]))
		(*j)++;
	return (false);
}

void	split_by_space_expand(char **str, t_list **words, int i, int start)
{
	t_list	*new;

	if (*words == NULL)
		*words = ft_xlstnew(ft_xsubstr(*str, start, i - start, "expansion"),
				"expansion");
	else
	{
		new = ft_xlstnew(ft_xsubstr(*str, start, i - start, "expansion"),
				"expansion");
		ft_lstadd_back(words, new);
	}
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

int	split_before_expanded(t_list **words, t_expand *data, char **head, int j)
{
	if (j > 0 && data->h_len > 0 && j != data->h_len)
		split_by_space_expand(head, words, data->h_len, 0);
	if (j != data->h_len)
		return (j);
	else
		return (0);
}

void	word_splitting(t_list **words, t_expand *data, char **head)
{
	int		j;
	int		start;

	j = 0;
	if (data->status == NONE && !is_allspace_or_null(data, head, &j))
	{
		start = split_before_expanded(words, data, head, j);
		while ((*head)[j] != '\0')
		{
			if (is_blank((*head)[j]))
			{
				split_by_space_expand(head, words, j, start);
				if (data->end && check_str_type(&(*head)[j]) == ALL_SPACE)
				{
					free(*head);
					return ;
				}
				while ((*head)[j + 1] != '\0' && is_blank((*head)[j + 1]))
					j++;
				start = j + 1;
			}
			j++;
		}
		get_new_head(head, j, start);
	}
}
