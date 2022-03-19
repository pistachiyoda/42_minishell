#include "minishell.h"

size_t	get_left_len(char *str, int i)
{
	char	*left;

	left = NULL;
	while (str[i + 1] != '\0')
	{
		if (str[i + 1] == '"' || str[i + 1] == '\'' || str[i + 1] == '$'
			|| is_blank(str[i + 1]))
		{
			left = &str[i + 1];
			break ;
		}
		i++;
	}
	if (left != NULL)
		return (ft_strlen(left));
	return (0);
}

void	expand_exit_status(t_expand *data, char **head)
{
	char	*param;
	char	*tmp;

	data->i += 1;
	param = ft_xitoa(g_status, "expansion");
	tmp = ft_xstrjoin(*head, param, "expansion");
	free(param);
	free(*head);
	*head = tmp;
}

void	add_dollar_mark(char **head, char *str)
{
	char	*tmp;

	tmp = *head;
	*head = ft_xstrjoin(tmp, str, "expansion");
	free(tmp);
}

char	*expand_env(t_environ *env, t_expand *data, char *str, char **head)
{
	char	*param;
	char	*value;
	char	*tmp;
	int		i;

	tmp = NULL;
	value = NULL;
	i = 0;
	param = ft_xsubstr(str, data->i + 1,
			ft_strlen(str) - (data->i + 1) - get_left_len(str, data->i),
			"expansion");
	data->i += ft_strlen(param);
	is_env_registered(env, &param, &value);
	if (value && data->status == NONE
		&& (ft_strlen(value) == 0 || check_str_type(value) != SPACELESS))
		data->rdr_error = true;
	if (data->status == NONE && !value && !(*head)
		&& data->i + 1 == (int)ft_strlen(str))
		data->rdr_error = true;
	else
		tmp = ft_xstrjoin(*head, value, "expansion");
	free(param);
	free(*head);
	*head = tmp;
	return (value);
}

void	param_expansion(t_environ *env, t_expand *data, char *str, char **head)
{
	char	*value;

	value = NULL;
	if (*head)
		data->h_len = ft_strlen(*head);
	if (ft_strncmp(&str[data->i], "$?", 2) == 0)
		expand_exit_status(data, head);
	else if (ft_strncmp(&str[data->i], "$$", 2) == 0)
	{
		data->i += 1;
		add_dollar_mark(head, "$$");
	}
	else if ((data->status == DQUOTE && (is_blank(str[data->i + 1])
				|| str[data->i + 1] == '"' || str[data->i + 1] == '\''))
		|| str[data->i + 1] == '\0')
		add_dollar_mark(head, "$");
	else
		value = expand_env(env, data, str, head);
	data->managed_i = data->i + 1;
	if (*head == NULL || (value && data->managed_i == (int)ft_strlen(str)
			&& is_space_at_end(*head)))
		data->end = true;
}
