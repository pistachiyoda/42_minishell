#include "minishell.h"

char	*is_env_registerd(t_environ *env, char **split_ele, bool key_only)
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
				env->value = split_ele[1];
			value = env->value;
		}
		env = env->next;
	}
	return (value);
}
