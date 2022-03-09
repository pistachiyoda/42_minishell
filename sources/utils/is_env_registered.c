#include "minishell.h"

char	*is_env_registerd(t_environ *env, char **split_ele, bool key_only,
	char *target)
{
	size_t	len;
	char	*value;

	len = ft_strlen(split_ele[0]);
	env = env->next;
	value = NULL;
	while (env->key != NULL)
	{
		if (ft_strlen(env->key) > len)
			len = ft_strlen(env->key);
		if (ft_strncmp(env->key, split_ele[0], len) == 0)
		{
			if (!key_only)
			{
				free(env->value);
				env->value = xstrdup(split_ele[1], target);
			}
			value = env->value;
		}
		env = env->next;
	}
	return (value);
}

bool	is_character_contained(char *str, int *i)
{
	while (str[*i] != '\0' && is_space_tab_newline(str[*i]))
		(*i)++;
	if (str[*i] == '\0')
		return (false);
	return (true);
}

bool	is_space_tab_newline(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (true);
	return (false);
}
