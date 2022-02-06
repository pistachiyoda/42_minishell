#include "minishell.h"

int	run_command(t_cmd_bolck *cmd_block)
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
