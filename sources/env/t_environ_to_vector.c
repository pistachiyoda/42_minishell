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
	env_vec = malloc(sizeof(char *) * (count_environ_ele(env) + 1));
	if (env_vec == NULL)
	{
		print_error("env_conversion", EMESS_MALLOC_FAIL);
		exit(EXIT_FAILURE);
	}
	env = env->next;
	while (env->key != NULL)
	{
		tmp = ft_strjoin(env->key, "=");
		env_vec[i++] = ft_strjoin(tmp, env->value);
		free(tmp);
		env = env->next;
	}
	env_vec[i] = NULL;
	return (env_vec);
}
