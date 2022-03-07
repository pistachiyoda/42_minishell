#include "minishell.h"

void	quote_removal(char *str, char **head, int i, int *prev_q)
{
	char	*front;
	char	*add;

	if (*head == NULL)
		*head = xsubstr(str, 0, i, "expansion");
	else if (ft_strnstr(&str[*prev_q], "$", i - *prev_q) == NULL)
	{
		add = xsubstr(str, *prev_q, i - *prev_q, "expansion");
		front = xstrjoin(*head, add, "expansion");
		free(add);
		free(*head);
		*head = front;
	}
	if (!ft_strchr(&str[i + 1], '$') && !ft_strchr(&str[i + 1], '\'')
		&& !ft_strchr(&str[i + 1], '"'))
	{
		front = xstrjoin(*head, &str[i + 1], "expansion");
		free(*head);
		*head = front;
	}
	*prev_q = i + 1;
}

void	set_head_before_dollar(char *str, char **head, int i, int prev_q)
{
	char	*front;
	char	*add;

	if (*head == NULL)
		*head = xsubstr(str, 0, i, "expansion");
	else if (prev_q != 0 && prev_q != i)
	{
		add = xsubstr(str, prev_q, i - prev_q, "expansion");
		front = xstrjoin(*head, add, "expansion");
		free(add);
		free(*head);
		*head = front;
	}
}
