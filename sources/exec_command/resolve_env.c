/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 01:15:20 by fmai              #+#    #+#             */
/*   Updated: 2022/03/23 01:15:20 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_NULL(char	*command, char *path_val)
{
	if (path_val == NULL)
	{
		print_error(command, EMESS_NO_FILE_DIR);
		exit(127);
	}
}

char	*resolve_path(char	*command, char *path_val)
{
	char	**paths;
	char	*path;
	char	*full_path;
	int		i;

	handle_NULL(command, path_val);
	paths = ft_split_wrapper(path_val, ':');
	i = 0;
	while (paths[i] != NULL)
	{
		path = ft_strjoin_wrapper(paths[i], "/");
		full_path = ft_strjoin_wrapper(path, command);
		free(path);
		if (is_executable(full_path))
		{
			free_2d_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_2d_array(paths);
	print_error(command, EMESS_NO_CMD);
	exit(127);
}
