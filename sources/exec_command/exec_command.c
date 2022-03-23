/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 01:15:20 by fmai              #+#    #+#             */
/*   Updated: 2022/03/23 01:15:20 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_relative_or_absolute(char *command_path)
{
	if (ft_strncmp(command_path, "/", 1) == 0
		|| ft_strncmp(command_path, "./", 2) == 0
		|| ft_strncmp(command_path, "../", 3) == 0)
		return (true);
	return (false);
}

void	exec_relative_or_absolute(char *command_path, char **args, char **envp)
{
	if (!is_exists(command_path))
	{
		print_error(command_path, EMESS_NO_FILE_DIR);
		exit(127);
	}
	if (!is_executable(command_path))
	{
		print_error(command_path, EMESS_NO_PERM);
		exit(126);
	}
	if (is_directory(command_path))
	{
		print_error(command_path, EMESS_IS_DIR);
		exit(126);
	}
	execve_wrapper(command_path, args, envp);
	exit(1);
}

void	handle_empty_command(char *command)
{
	if (command[0] == '\0')
	{
		print_error("", EMESS_NO_CMD);
		exit(127);
	}
}

void	exec_command(t_cmd_block *cmd_block, char **envp)
{
	char		*path_val;
	char		*real_command_path;
	t_environ	*env;
	int			result;

	if (cmd_block->command == NULL)
		exit (0);
	if (is_relative_or_absolute(cmd_block->command))
		exec_relative_or_absolute(cmd_block->command, cmd_block->args, envp);
	if (is_builtin_command(cmd_block))
	{
		env = create_environ(envp);
		result = run_builtin_command(cmd_block, env);
		free_env(env);
		exit(result);
	}
	handle_empty_command(cmd_block->command);
	path_val = get_env_val("PATH", envp);
	real_command_path = resolve_path(cmd_block->command, path_val);
	if (!real_command_path)
		exit(1);
	free(path_val);
	execve_wrapper(real_command_path, cmd_block->args, envp);
	exit(1);
}
