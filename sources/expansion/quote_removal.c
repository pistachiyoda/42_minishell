#include "minishell.h"

void	concat_normal_str(char *str, char **head, int i, t_quote *quote)
{
	char	*front;
	char	*add;

	add = xsubstr(str, quote->prev_q, i - quote->prev_q, "expansion");
	front = xstrjoin(*head, add, "expansion");
	free(add);
	free(*head);
	*head = front;
}

void	concat_expanded_and_left(char *str, char **head, int i, t_quote *quote)
{
	char	*front;
	char	*add;

	add = xsubstr(str, quote->managed_i, i - quote->managed_i, "expansion");
	front = xstrjoin(*head, add, "expansion");
	free(add);
	free(*head);
	*head = front;
}

void	concat_all(char *str, char **head, int i)
{
	char	*front;

	if (!ft_strchr(&str[i + 1], '$') && !ft_strchr(&str[i + 1], '\'')
		&& !ft_strchr(&str[i + 1], '"') && str[i + 1] != '\0')
	{
		front = xstrjoin(*head, &str[i + 1], "expansion");
		free(*head);
		*head = front;
	}
}

void	quote_removal(char *str, char **head, int i, t_quote *quote)
{
	if (*head == NULL)
		*head = xsubstr(str, 0, i, "expansion");
	else if (ft_strnstr(&str[quote->prev_q], "$", i - quote->prev_q) == NULL
		|| (ft_strnstr(&str[quote->prev_q], "$", i - quote->prev_q)
			&& str[i] == '\'' && quote->status == NONE))
		concat_normal_str(str, head, i, quote);
	else if (ft_strnstr(&str[quote->prev_q], "$", i - quote->prev_q)
		&& str[i] == '"' && quote->status == NONE && i != quote->prev_q)
		concat_expanded_and_left(str, head, i, quote);
	concat_all(str, head, i);
	quote->prev_q = i + 1;
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
