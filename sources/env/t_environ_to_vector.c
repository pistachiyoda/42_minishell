#include "minishell.h"

int	count_environ_ele(t_environ *env)
{
	int	count;

	count = 0;
	env = env->next;
	while (env->key != NULL)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char	**t_environ_to_vector(t_environ *env)
{
	int		i;
	char	**env_vec;
	char	*tmp;

	i = 0;
	env_vec = ft_xmalloc(sizeof(char *) * (count_environ_ele(env) + 1),
			"env_conversion");
	env = env->next;
	while (env->key != NULL)
	{
		if (env->value)
		{
			tmp = ft_xstrjoin(env->key, "=", "env_conversion");
			env_vec[i++] = ft_xstrjoin(tmp, env->value, "env_conversion");
			free(tmp);
		}
		else
			env_vec[i++] = ft_xstrdup(env->key, "env_conversion");
		env = env->next;
	}
	env_vec[i] = NULL;
	return (env_vec);
}
