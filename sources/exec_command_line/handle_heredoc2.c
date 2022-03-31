/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 01:15:20 by fmai              #+#    #+#             */
/*   Updated: 2022/03/31 20:48:27 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_env_variables(t_environ *env, char *key)
{
	char	*ret;
	char	**envp;

	envp = t_environ_to_vector(env);
	ret = get_env_val(key, envp);
	free(key);
	free_2d_array(envp);
	if (ret == NULL)
		return (ft_xstrdup("", "expand_env_variables_in_buf"));
	return (ret);
}

bool	is_special_char_for_heredoc(char c)
{
	if (c != ' ' && c != '\n' && c != '\0'
		&& c != '\"' && c != '\'' && c != '$'
		&& (ft_isalnum(c) == 1 || c == '_'))
		return (false);
	return (true);
}

char	*get_key(char *buf, int *i)
{
	int		cnt;
	char	*key;
	int		j;

	cnt = 0;
	(*i)++;
	while (!is_special_char_for_heredoc(buf[*i]))
	{
		cnt ++;
		(*i)++;
	}
	key = ft_xmalloc(sizeof(char *) * (cnt + 1), "get_key");
	(*i) -= cnt;
	j = 0;
	while (j < cnt)
	{
		key[j] = buf[*i];
		j ++;
		(*i)++;
	}
	key[j] = '\0';
	(*i)--;
	return (key);
}

char	*get_expanded_value(char *buf, int *i, t_environ *env)
{
	if (buf[(*i) + 1] == '?')
	{
		(*i)++;
		return(ft_xitoa(g_status, "expand_env"));
	}
	else if (is_special_char_for_heredoc(buf[(*i) + 1]))
		return(ft_xstrdup("$", "expand_env"));
	else
		return(expand_env_variables(env, get_key(buf, i)));
}

char	*expand_env_variables_in_buf(t_environ *env, char *buf)
{
	int		i;
	char	*tmp_str;
	char	*value;
	char	*joined_str;

	i = 0;
	joined_str = ft_xstrdup("", "expand_env");
	while (buf[i])
	{
		if (buf[i] == '$')
		{
			value = get_expanded_value(buf, &i, env);
			joined_str = ft_xstrjoin_with_free(joined_str, value, "expand_env");
		}
		else
		{
			tmp_str = ft_xmalloc(sizeof(char *) * 2, "expand_env");
			tmp_str[0] = buf[i];
			tmp_str[1] = '\0';
			joined_str = ft_xstrjoin_with_free(
					joined_str, tmp_str, "expand_env");
		}
		i++;
	}
	return (joined_str);
}
