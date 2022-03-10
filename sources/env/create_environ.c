#include "minishell.h"

t_environ	*init_environ(char *target)
{
	t_environ	*env;

	env = (t_environ *)malloc(sizeof(t_environ));
	if (env == NULL)
	{
		print_error(target, EMESS_MALLOC_FAIL);
		exit(EXIT_FAILURE);
	}
	env->key = NULL;
	env->value = NULL;
	env->prev = env;
	env->next = env;
	return (env);
}

t_environ	*add_environ(t_environ *env, t_environ *first_ele,
							char **split_ele, char *target)
{
	t_environ	*new_ele;

	new_ele = (t_environ *)malloc(sizeof(t_environ));
	if (new_ele == NULL)
	{
		print_error(target, EMESS_MALLOC_FAIL);
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

char	**split_by_delimiter(char *str, bool *key_only, char *target)
{
	char	**split_ele;
	char	*value;
	size_t	len;

	len = 0;
	value = ft_strchr(str, '=');
	if (!value)
		*key_only = true;
	else
	{
		len = ft_strlen(value);
		value = xstrdup(&value[1], target);
	}
	split_ele = (char **)xmalloc(sizeof(char *) * 3, target);
	split_ele[0] = xsubstr(str, 0, ft_strlen(str) - len, target);
	split_ele[1] = value;
	split_ele[2] = NULL;
	return (split_ele);
}

t_environ	*create_environ(char **envp)
{
	int			i;
	t_environ	*env;
	t_environ	*first_ele;
	char		*target;
	bool		flag;

	i = 0;
	flag = true;
	target = "create_environ";
	env = init_environ(target);
	first_ele = env;
	while (envp[i] != NULL)
	{
		env = add_environ(env, first_ele,
				split_by_delimiter(envp[i], &flag, target), target);
		i++;
	}
	return (env = env->next);
}
