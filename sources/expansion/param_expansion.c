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

int	param_expansion(t_environ *env, char *str, char **head, int *i)
{
	char	*param;
	char	*tmp;

	if (ft_strncmp(&str[*i], "$?", 2) == 0)
	{
		*i += 1;
		param = xitoa(g_status, "expansion");
		tmp = xstrjoin(*head, param, "expansion");
	}
	else
	{
		param = xsubstr(str, *i + 1,
				ft_strlen(str) - (*i + 1) - get_left_len(str, *i), "expansion");
		*i += ft_strlen(param);
		tmp = xstrjoin(*head, is_env_registered(env, &param, true, NULL),
				"expansion");
	}
	free(param);
	free(*head);
	*head = tmp;
	return (*i + 1);
}
