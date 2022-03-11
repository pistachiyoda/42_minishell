#include "minishell.h"

size_t	get_left_len(char *str, int i)
{
	char	*left;

	left = NULL;
	while (str[i + 1] != '\0')
	{
		if (str[i + 1] == '"' || str[i + 1] == '\'' || str[i + 1] == '$'
			|| is_space_tab_newline(str[i + 1]))
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

bool	is_space_at_end(char *str)
{
	size_t	i;

	i = ft_strlen(str);
	if (is_space_tab_newline(str[i - 1]))
		return (true);
	return (false);
}

void	param_expansion(t_environ *env, t_expand *data, char *str, char **head)
{
	char	*param;
	char	*tmp;

	if (ft_strncmp(&str[data->i], "$?", 2) == 0)
	{
		data->i += 1;
		param = xitoa(g_status, "expansion");
		tmp = xstrjoin(*head, param, "expansion");
	}
	else
	{
		param = xsubstr(str, data->i + 1,
				ft_strlen(str) - (data->i + 1) - get_left_len(str, data->i),
				"expansion");
		data->i += ft_strlen(param);
		tmp = xstrjoin(*head, is_env_registered(env, &param, true, NULL),
				"expansion");
	}
	free(param);
	free(*head);
	*head = tmp;
	data->managed_i = data->i + 1;
	if (data->managed_i == (int)ft_strlen(str) && is_space_at_end(*head))
		data->end = true;
}
