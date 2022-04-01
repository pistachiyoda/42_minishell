/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 23:03:08 by fmai              #+#    #+#             */
/*   Updated: 2022/04/01 15:28:39 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_double_slash(char *command_path)
{
	if (ft_strncmp(command_path, "//", 2) == 0
		&& ft_strncmp(command_path, "///", 3) != 0)
		return (true);
	return (false);
}

bool	is_absolute(char *command_path)
{
	if (ft_strncmp(command_path, "/", 1) == 0)
		return (true);
	return (false);
}

int	chdir_wrapper(char *path)
{
	char		*e_mes;
	int			ret;

	ret = chdir(path);
	if (ret == -1)
	{
		e_mes = ft_strjoin_wrapper("minishell: cd: ", path);
		perror(e_mes);
		free(e_mes);
		return (1);
	}
	return (0);
}
