/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 23:03:08 by fmai              #+#    #+#             */
/*   Updated: 2022/03/21 23:03:08 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(char	*command, char *builtin)
{
	int	builtin_length;
	int	command_length;

	builtin_length = ft_strlen(builtin);
	command_length = ft_strlen(command);
	if ((builtin_length == command_length)
		&& ft_strncmp(command, builtin, builtin_length) == 0)
		return (true);
	return (false);
}

bool	is_builtin_command(t_cmd_block *cmd_block)
{
	return (is_builtin(cmd_block->command, "echo")
		|| is_builtin(cmd_block->command, "cd")
		|| is_builtin(cmd_block->command, "pwd")
		|| is_builtin(cmd_block->command, "export")
		|| is_builtin(cmd_block->command, "unset")
		|| is_builtin(cmd_block->command, "env")
		|| is_builtin(cmd_block->command, "exit"));
}

bool	is_fork_required(t_list *cmd_list)
{
	t_cmd_block	*cmd_block;

	cmd_block = (t_cmd_block *)cmd_list->content;
	if (ft_lstsize(cmd_list) == 1
		&& cmd_block->command
		&& is_builtin_command(cmd_block))
		return (false);
	return (true);
}

int	run_builtin_command(t_cmd_block *cmd_block, t_environ *env)
{
	if (is_builtin(cmd_block->command, "echo"))
		return (ft_echo(cmd_block));
	if (is_builtin(cmd_block->command, "cd"))
		return (ft_cd(cmd_block, env));
	if (is_builtin(cmd_block->command, "pwd"))
		return (ft_pwd(env));
	if (ft_strncmp(cmd_block->command, "export", 6) == 0)
		return (ft_export(cmd_block, env));
	if (ft_strncmp(cmd_block->command, "unset", 5) == 0)
		return (ft_unset(cmd_block, env));
	if (ft_strncmp(cmd_block->command, "env", 3) == 0)
		return (ft_env(env));
	if (is_builtin(cmd_block->command, "exit"))
		return (ft_exit(cmd_block));
	return (0);
}
