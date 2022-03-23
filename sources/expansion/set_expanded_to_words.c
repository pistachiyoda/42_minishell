/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_expanded_to_words.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 23:03:08 by fmai              #+#    #+#             */
/*   Updated: 2022/03/21 23:03:08 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_to_words(t_list **words, char *head, char *str)
{
	if (head == NULL)
		head = ft_xstrdup(str, "expansion");
	if (*words == NULL)
		*words = ft_xlstnew(head, "expansion");
	else
		ft_lstadd_back(words, ft_xlstnew(head, "expansion"));
}

t_expand	init_struct(void)
{
	t_expand	data;

	data.prev_q = 0;
	data.status = NONE;
	data.i = 0;
	data.managed_i = 0;
	data.h_len = 0;
	data.end = false;
	data.rdr_error = false;
	return (data);
}

bool	set_expanded_to_words(
	t_environ *env, char *str, t_list **words, int type)
{
	t_expand	data;
	char		*head;

	data = init_struct();
	head = NULL;
	while (str[data.i] != '\0')
	{
		if (is_quote_type_switched(str, data.i, &data.status))
			quote_removal(str, &head, &data, type);
		if (data.status != QUOTE && str[data.i] == '$'
			&& type != QUOTED_HEREDOC)
		{
			set_head_before_dollar(str, &head, data);
			param_expansion(env, &data, str, &head);
			word_splitting(words, &data, &head);
		}
		data.i++;
	}
	if (!data.end)
		add_to_words(words, head, str);
	return (data.rdr_error);
}
