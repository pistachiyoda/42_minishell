/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 23:03:08 by fmai              #+#    #+#             */
/*   Updated: 2022/03/21 23:03:08 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_arg(char *str, char *original, char *cmd)
{
	int	j;

	j = 0;
	while (str[j] != '\0')
	{
		if (j == 0 && ft_isalpha(str[0]) != 1 && str[0] != '_')
			break ;
		if (j != 0 && ft_isalnum(str[j]) != 1 && str[j] != '_')
			break ;
		j++;
	}
	if (str[j] != '\0' || ft_strlen(str) == 0)
	{
		invalid_id_error(original, cmd);
		return (false);
	}
	return (true);
}
