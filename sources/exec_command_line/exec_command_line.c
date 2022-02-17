#include "minishell.h"
int	exec_command_line(t_list *cmd_list, char **envp)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == 0)
	{
		exec_command(((t_cmd_block *)cmd_list->content)->command,
			((t_cmd_block *)cmd_list->content)->args, envp);
	}
	waitpid(pid, &status, 0);
	return (1);
}
