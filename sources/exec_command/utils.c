/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 22:11:48 by fmai              #+#    #+#             */
/*   Updated: 2022/03/09 22:11:48 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
	{
		perror("stat()");
		exit(1);
	}
	return (S_ISDIR(st.st_mode));
}

bool	is_exists(char *path)
{
	if (access(path, F_OK) == 0)
		return (true);
	return (false);
}

bool	is_executable(char *command_path)
{
	if (access(command_path, X_OK) == 0)
		return (true);
	return (false);
}
