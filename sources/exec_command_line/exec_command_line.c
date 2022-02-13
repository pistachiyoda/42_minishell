#include "minishell.h"

// cpputestでこの関数単体でのテストする
int	exec_command_line(t_cmd_block *cmd_block, char **envp)
{
	exec_command(cmd_block->command, cmd_block->args, envp);
	return (1);
}
