#include "minishell.h"

char	*is_env_registered(
	t_environ *env, char **split_ele, bool key_only, char *target)
{
	size_t	len;
	size_t	len_env;
	char	*value;

	env = env->next;
	value = NULL;
	len = ft_strlen(split_ele[0]);
	while (env->key != NULL)
	{
		len_env = ft_strlen(env->key);
		if (len_env < len)
			len_env = len;
		if (ft_strncmp(env->key, split_ele[0], len_env) == 0)
		{
			if (!key_only)
			{
				free(env->value);
				env->value = ft_xstrdup(split_ele[1], target);
			}
			value = env->value;
		}
		env = env->next;
	}
	return (value);
}
