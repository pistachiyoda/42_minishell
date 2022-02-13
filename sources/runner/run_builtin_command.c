#include "minishell.h"

bool	is_fork_required(t_list *cmd_list)
{
	t_cmd_block *cmd_block;
	
	cmd_block = (t_cmd_block *)cmd_list->content;
	if (ft_lstsize(cmd_list) == 1
		&& (ft_strncmp(cmd_block->command, "echo", 4) == 0
			|| ft_strncmp(cmd_block->command, "cd", 2) == 0
			|| ft_strncmp(cmd_block->command, "pwd", 3) == 0
			|| ft_strncmp(cmd_block->command, "export", 6) == 0
			|| ft_strncmp(cmd_block->command, "unset", 5) == 0
			|| ft_strncmp(cmd_block->command, "env", 3) == 0
			|| ft_strncmp(cmd_block->command, "exit", 4) == 0))
		return (false);
	return (true);
}

int	run_builtin_command(t_cmd_block *cmd_block)
{
	if (ft_strncmp(cmd_block->command, "echo", 4) == 0)
		ft_putstr_fd(cmd_block->command, 1);
	if (ft_strncmp(cmd_block->command, "cd", 2) == 0)
		ft_putstr_fd(cmd_block->command, 1);
	if (ft_strncmp(cmd_block->command, "pwd", 3) == 0)
		ft_putstr_fd(cmd_block->command, 1);
	if (ft_strncmp(cmd_block->command, "export", 6) == 0)
		ft_putstr_fd(cmd_block->command, 1);
	if (ft_strncmp(cmd_block->command, "unset", 5) == 0)
		ft_putstr_fd(cmd_block->command, 1);
	if (ft_strncmp(cmd_block->command, "env", 3) == 0)
		ft_putstr_fd(cmd_block->command, 1);
	if (ft_strncmp(cmd_block->command, "exit", 4) == 0)
		ft_putstr_fd(cmd_block->command, 1);
	ft_putchar_fd('\n', 1);
	return (0);
}
