#include "minishell.h"

size_t	get_left(char *str, int i, char **left)
{
	while (str[i + 1] != '\0')
	{
		if (str[i + 1] == '"' || str[i + 1] == '\'' || str[i + 1] == '$')
		{
			*left = &str[i + 1];
			break ;
		}
		i++;
	}
	if (*left != NULL)
		return (ft_strlen(*left));
	return (0);
}

char	*param_expansion(t_environ *env, char *str, char **head, int *i)
{
	char	*param;
	char	*left;
	char	*tmp;

	left = NULL;
	param = xsubstr(str, *i + 1,
			ft_strlen(str) - (*i + 1) - get_left(str, *i, &left),
			"expansion");
	*i += ft_strlen(param);
	tmp = xstrjoin(*head, is_env_registerd(env, &param, true), "expansion");
	free(param);
	free(*head);
	*head = tmp;
	return (left);
}
