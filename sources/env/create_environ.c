#include "minishell.h"

t_environ	*init_environ(char *msg)
{
	t_environ	*env;

	env = (t_environ *)malloc(sizeof(t_environ));
	if (env == NULL)
	{
		print_error(msg, EMESS_MALLOC_FAIL);
		exit(EXIT_FAILURE);
	}
	env->key = NULL;
	env->value = NULL;
	env->prev = env;
	env->next = env;
	return (env);
}

char	**split_by_delimiter(char *env_str, char *msg)
{
	char		**split_ele;

	split_ele = ft_split(env_str, '=');
	if (split_ele == NULL)
	{
		print_error(msg, EMESS_MALLOC_FAIL);
		exit(EXIT_FAILURE);
	}
	return (split_ele);
}

t_environ	*add_environ(t_environ *env, t_environ *first_ele,
							char **split_ele, char *msg)
{
	t_environ	*new_ele;

	new_ele = (t_environ *)malloc(sizeof(t_environ));
	if (new_ele == NULL)
	{
		print_error(msg, EMESS_MALLOC_FAIL);
		exit(EXIT_FAILURE);
	}
	new_ele->key = split_ele[0];
	new_ele->value = split_ele[1];
	new_ele->prev = env;
	new_ele->next = first_ele;
	first_ele->prev = new_ele;
	env->next = new_ele;
	return (env = env->next);
}

t_environ	*create_environ(char **envp)
{
	int			i;
	t_environ	*env;
	t_environ	*first_ele;
	char		*msg;

	i = 0;
	msg = "create_environ";
	env = init_environ(msg);
	first_ele = env;
	while (envp[i] != NULL)
	{
		env = add_environ(env, first_ele,
				split_by_delimiter(envp[i], msg), msg);
		i++;
	}
	return (env = env->next);
}
