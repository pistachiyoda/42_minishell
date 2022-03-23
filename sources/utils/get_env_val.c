/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_val.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 23:03:08 by fmai              #+#    #+#             */
/*   Updated: 2022/03/21 23:03:08 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**set_data(char	*str, int key_cnt, int val_cnt)
{
	char	**data;

	data = ft_xmalloc(sizeof(char *) * 3, "get_env_val");
	data[0] = ft_xmalloc(sizeof(char) * (key_cnt + 1), "get_env_val");
	data[1] = ft_xmalloc(sizeof(char) * (val_cnt + 1), "get_env_val");
	data[2] = NULL;
	ft_memcpy(data[0], str, key_cnt);
	str += key_cnt;
	data[0][key_cnt] = '\0';
	str ++;
	ft_memcpy(data[1], str, val_cnt);
	str += val_cnt;
	data[1][val_cnt] = '\0';
	return (data);
}

char	**split_into_two(char *str, char splitter)
{
	int		i;
	int		key_cnt;
	int		val_cnt;
	bool	is_key;

	i = 0;
	key_cnt = 0;
	val_cnt = 0;
	is_key = true;
	while (str[i])
	{
		if (is_key && str[i] == splitter)
		{
			is_key = false;
			i ++;
			continue ;
		}
		if (is_key)
			key_cnt ++;
		else
			val_cnt ++;
		i ++;
	}
	return (set_data(str, key_cnt, val_cnt));
}

// 最初の=で二つに分割する
char	*get_env_val(char *key, char **envp)
{
	int		i;
	char	**data;
	char	*env_val;

	i = 0;
	while (envp[i] != NULL)
	{
		data = split_into_two(envp[i], '=');
		if (ft_strlen(data[0]) == ft_strlen(key)
			&& ft_strncmp(envp[i], key, ft_strlen(key)) == 0)
		{
			env_val = ft_xstrdup(data[1], "get_env_val");
			free_2d_array(data);
			return (env_val);
		}
		free_2d_array(data);
		i++;
	}
	return (NULL);
}
