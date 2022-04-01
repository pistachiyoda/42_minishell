/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasubuc <mmasubuc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 23:03:08 by fmai              #+#    #+#             */
/*   Updated: 2022/04/01 13:52:25 by mmasubuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_dollar_mark(char **head, char *str)
{
	char	*tmp;

	tmp = *head;
	*head = ft_xstrjoin(tmp, str, "expansion");
	free(tmp);
}

void	add_before_dollar_quote(t_expand *data, char *str, char **head)
{
	char	*tmp;
	char	*left;
	int		j;

	j = 0;
	if (data->status == NONE)
	{
		while (str[data->i + 1 + j] != '\0' && str[data->i + 1 + j] != '$'
			&& str[data->i + 1 + j] != '\'' && str[data->i + 1 + j] != '"')
			j++;
		if (j != 0)
		{
			left = ft_xsubstr(str, data->i + 1, j, "expansion");
			tmp = *head;
			*head = ft_xstrjoin(tmp, left, "expansion");
			free(tmp);
			free(left);
		}
		data->i += j;
	}
}

void	param_expansion(t_environ *env, t_expand *data, char *str, char **head)
{
	char	*value;

	value = NULL;
	if (*head)
		data->h_len = ft_strlen(*head);
	if (ft_strncmp(&str[data->i], "$?", 2) == 0)
		expand_exit_status(data, head);
	else if (ft_strncmp(&str[data->i], "$$", 2) == 0)
	{
		data->i += 1;
		add_dollar_mark(head, "$$");
	}
	else if ((data->status == DQUOTE && (is_blank(str[data->i + 1])
				|| str[data->i + 1] == '"' || str[data->i + 1] == '\''))
		|| str[data->i + 1] == '\0' || is_special_char(str[data->i + 1]))
		add_dollar_mark(head, "$");
	else
		value = expand_env(env, data, str, head);
	add_before_dollar_quote(data, str, head);
	data->managed_i = data->i + 1;
	if (*head == NULL || (value && data->managed_i == (int)ft_strlen(str)
			&& ft_strlen(value) != 0 && is_space_at_end(*head)))
		data->end = true;
}
