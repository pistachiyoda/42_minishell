#include "minishell.h"

void	concat_normal_str(char *str, char **head, t_expand *data)
{
	char	*front;
	char	*add;

	add = ft_xsubstr(str, data->prev_q, data->i - data->prev_q, "expansion");
	front = ft_xstrjoin(*head, add, "expansion");
	free(add);
	free(*head);
	*head = front;
}

void	concat_expanded_and_left(char *str, char **head, t_expand *data)
{
	char	*front;
	char	*add;

	add = ft_xsubstr(str, data->managed_i, data->i - data->managed_i,
			"expansion");
	front = ft_xstrjoin(*head, add, "expansion");
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
		front = ft_xstrjoin(*head, &str[i + 1], "expansion");
		free(*head);
		*head = front;
	}
}

void	quote_removal(char *str, char **head, t_expand *data, int type)
{
	if (*head == NULL)
		*head = ft_xsubstr(str, 0, data->i, "expansion");
	else if (ft_strnstr(&str[data->prev_q], "$", data->i - data->prev_q) == NULL
		|| (ft_strnstr(&str[data->prev_q], "$", data->i - data->prev_q)
			&& str[data->i] == '\'' && data->status == NONE)
		|| type == QUOTED_HEREDOC)
		concat_normal_str(str, head, data);
	else if (ft_strnstr(&str[data->prev_q], "$", data->i - data->prev_q) \
	&& str[data->i] == '"' && data->status == NONE && data->i != data->prev_q)
		concat_expanded_and_left(str, head, data);
	concat_all(str, head, data->i);
	data->prev_q = data->i + 1;
}

void	set_head_before_dollar(char *str, char **head, t_expand data)
{
	char	*front;
	char	*add;

	if (*head == NULL)
		*head = ft_xsubstr(str, 0, data.i, "expansion");
	else if (data.prev_q != 0 && data.prev_q != data.i)
	{
		add = ft_xsubstr(str, data.prev_q, data.i - data.prev_q, "expansion");
		front = ft_xstrjoin(*head, add, "expansion");
		free(add);
		free(*head);
		*head = front;
	}
}
