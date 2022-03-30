/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_str_type.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasubuc <mmasubuc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 23:03:08 by fmai              #+#    #+#             */
/*   Updated: 2022/03/30 18:50:40 by mmasubuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_str_type(char *str)
{
	int	i;
	int	status;
	int	count;

	i = 0;
	status = SPACELESS;
	count = 0;
	while (str[i] != '\0')
	{
		if (is_blank(str[i]))
		{
			status = PARTIAL_SPACE;
			count++;
		}
		i++;
	}
	if (i != 0 && count == i)
		status = ALL_SPACE;
	return (status);
}

bool	is_character_contained(char *str, int *i)
{
	while (str[*i] != '\0' && is_blank(str[*i]))
		(*i)++;
	if (str[*i] == '\0')
		return (false);
	return (true);
}

bool	is_space_at_end(char *str)
{
	size_t	i;

	i = ft_strlen(str);
	if (i == 0 || is_blank(str[i - 1]))
		return (true);
	return (false);
}
