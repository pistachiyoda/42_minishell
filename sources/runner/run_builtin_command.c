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
		ft_putstr_fd(cmd_block->command, 1);
	if (is_builtin(cmd_block->command, "cd"))
		ft_putstr_fd(cmd_block->command, 1);
	if (is_builtin(cmd_block->command, "pwd"))
		return ft_pwd();
	if (is_builtin(cmd_block->command, "export"))
		ft_export(cmd_block, env);
	if (is_builtin(cmd_block->command, "unset"))
		ft_unset(cmd_block, env);
	if (is_builtin(cmd_block->command, "env"))
		ft_env(env);
	if (is_builtin(cmd_block->command, "exit"))
		ft_putstr_fd(cmd_block->command, 1);
	return (0);
}
