/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasubuc <mmasubuc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 21:19:45 by mmasubuc          #+#    #+#             */
/*   Updated: 2022/03/30 21:41:33 by mmasubuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_exit_status(t_expand *data, char **head)
{
	char	*param;
	char	*tmp;

	data->i += 1;
	param = ft_xitoa(g_status, "expansion");
	tmp = ft_xstrjoin(*head, param, "expansion");
	free(param);
	free(*head);
	*head = tmp;
}

size_t	get_left_len(char *str, int i)
{
	char	*left;

	left = NULL;
	while (str[i + 1] != '\0')
	{
		if (ft_isalnum(str[i + 1]) != 1 && str[i + 1] != '_')
		{
			left = &str[i + 1];
			break ;
		}
		i++;
	}
	if (left != NULL)
		return (ft_strlen(left));
	return (0);
}

char	*expand_env(t_environ *env, t_expand *data, char *str, char **head)
{
	char	*param;
	char	*value;
	char	*tmp;
	int		i;

	tmp = NULL;
	value = NULL;
	i = 0;
	param = ft_xsubstr(str, data->i + 1,
			ft_strlen(str) - (data->i + 1) - get_left_len(str, data->i),
			"expansion");
	data->i += ft_strlen(param);
	is_env_registered(env, &param, &value);
	if (value && data->status == NONE
		&& (ft_strlen(value) == 0 || check_str_type(value) != SPACELESS))
		data->rdr_error = true;
	if (data->status == NONE && !value && !(*head)
		&& data->i + 1 == (int)ft_strlen(str))
		data->rdr_error = true;
	else
		tmp = ft_xstrjoin(*head, value, "expansion");
	free(param);
	free(*head);
	*head = tmp;
	return (value);
}
