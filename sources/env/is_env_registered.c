#include "minishell.h"

void	update_value(t_environ *env, char **split_ele, char *target)
{
	size_t	len;
	size_t	len_env;

	env = env->next;
	len = ft_strlen(split_ele[0]);
	while (env->key != NULL)
	{
		len_env = ft_strlen(env->key);
		if (len == len_env && ft_strncmp(env->key, split_ele[0], len_env) == 0)
		{
			free(env->value);
			env->value = ft_xstrdup(split_ele[1], target);
		}
		env = env->next;
	}
}

bool	is_env_registered(t_environ *env, char **split_ele, char **value)
{
	size_t	len;
	size_t	len_env;
	bool	flag;

	env = env->next;
	len = ft_strlen(split_ele[0]);
	flag = false;
	while (env->key != NULL)
	{
		len_env = ft_strlen(env->key);
		if (len == len_env && ft_strncmp(env->key, split_ele[0], len) == 0)
		{
			*value = env->value;
			flag = true;
		}
		env = env->next;
	}
	return (flag);
}
