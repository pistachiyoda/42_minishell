#include "minishell.h"

int	ft_env(t_environ *env)
{
	env = env->next;
	while (env->key != NULL)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}
