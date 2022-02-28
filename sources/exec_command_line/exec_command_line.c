#include "minishell.h"

void	handle_command_line(t_list *cmd_list, char **envp)
{
	int	pipe_fds[FD_MAX][2];

	if (handle_redirect((t_cmd_block *)cmd_list->content, pipe_fds) != 0)
		exit(1);
	exec_command(((t_cmd_block *)cmd_list->content)->command,
		((t_cmd_block *)cmd_list->content)->args, envp);
	exit(1);
}

int	exec_command_line(t_list *cmd_list, char **envp)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == 0)
		handle_command_line(cmd_list, envp);
	waitpid(pid, &status, 0);
	return (1);
}
