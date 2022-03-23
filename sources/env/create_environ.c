/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_environ.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 01:15:20 by fmai              #+#    #+#             */
/*   Updated: 2022/03/23 01:15:20 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_environ	*init_environ(char *target)
{
	t_environ	*env;

	env = (t_environ *)ft_xmalloc(sizeof(t_environ), target);
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

	new_ele = (t_environ *)ft_xmalloc(sizeof(t_environ), target);
	new_ele->key = split_ele[0];
	new_ele->value = split_ele[1];
	new_ele->prev = env;
	new_ele->next = first_ele;
	first_ele->prev = new_ele;
	env->next = new_ele;
	free(split_ele);
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
		value = ft_xstrdup(&value[1], target);
	}
	split_ele = (char **)ft_xmalloc(sizeof(char *) * 3, target);
	split_ele[0] = ft_xsubstr(str, 0, ft_strlen(str) - len, target);
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
