/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 23:03:08 by fmai              #+#    #+#             */
/*   Updated: 2022/03/21 23:03:08 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset_env(char *str, t_environ *env, size_t len)
{
	t_environ	*tmp;

	while (env->key != NULL)
	{
		if (len < ft_strlen(env->key))
			len = ft_strlen(env->key);
		if (ft_strncmp(env->key, str, len) == 0)
		{
			env->prev->next = env->next;
			env->next->prev = env->prev;
			tmp = env;
			env = env->prev;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
		}
		env = env->next;
	}
}

int	ft_unset(t_cmd_block *cmd_block, t_environ *env)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (cmd_block->args[i] != NULL)
	{
		env = env->next;
		if (is_valid_arg(cmd_block->args[i], cmd_block->args[i], "unset"))
			unset_env(cmd_block->args[i], env, ft_strlen(cmd_block->args[i]));
		else
			status = 1;
		i++;
	}
	return (status);
}
