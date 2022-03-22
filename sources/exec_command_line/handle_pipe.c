#include "minishell.h"

int	handle_first_block(t_cmd_block *cmd_block, char	**envp, int pipe_write[2])
{
	int	pid;
	int	status;

	pipe_wrapper(pipe_write);
	set_signal(SIG_IGN, SIG_IGN);
	pid = fork_wrapper();
	if (pid == 0)
	{
		set_signal(SIG_DFL, SIG_DFL);
		close_wrapper(pipe_write[0]);
		if (dup2_wrapper(pipe_write[1], 1) != 0)
			exit(1);
		close_wrapper(pipe_write[1]);
		status = handle_redirects(cmd_block);
		if (status != 0)
			exit(status);
		exec_command(cmd_block, envp);
	}
	close_doc_pipe_fd(cmd_block);
	return (pid);
}

int	handle_middle_block(
	t_cmd_block *cmd_block, char **envp, int pipe_read[2], int pipe_write[2])
{
	int	pid;
	int	status;

	pipe_wrapper(pipe_write);
	pid = fork_wrapper();
	if (pid == 0)
	{
		set_signal(SIG_DFL, SIG_DFL);
		close_wrapper(pipe_read[1]);
		close_wrapper(pipe_write[0]);
		if (dup2_wrapper(pipe_read[0], 0) != 0)
			exit(1);
		if (dup2_wrapper(pipe_write[1], 1) != 0)
			exit(1);
		close_wrapper(pipe_read[0]);
		close_wrapper(pipe_write[1]);
		status = handle_redirects(cmd_block);
		if (status != 0)
			exit(status);
		exec_command(cmd_block, envp);
	}
	close_wrapper(pipe_read[0]);
	close_wrapper(pipe_read[1]);
	close_doc_pipe_fd(cmd_block);
	return (pid);
}

int	handle_last_block(t_cmd_block *cmd_block, char	**envp, int pipe_read[2])
{
	int	pid;
	int	status;

	pid = fork_wrapper();
	if (pid == 0)
	{
		set_signal(SIG_DFL, SIG_DFL);
		close_wrapper(pipe_read[1]);
		if (dup2_wrapper(pipe_read[0], 0) != 0)
			exit (1);
		close_wrapper(pipe_read[0]);
		status = handle_redirects(cmd_block);
		if (status != 0)
			exit(status);
		exec_command(cmd_block, envp);
	}
	close_wrapper(pipe_read[0]);
	close_wrapper(pipe_read[1]);
	close_doc_pipe_fd(cmd_block);
	return (pid);
}
