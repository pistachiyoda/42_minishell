#include "minishell.h"

int	handle_single_block(t_cmd_block *cmd_block, char **envp)
{
	int			pid;
	int			status;

	pid = fork_wrapper();
	if (pid == 0)
	{
		status = handle_redirects(cmd_block);
		if (status != 0)
			exit(status);
		exec_command(cmd_block, envp);
	}
	close_doc_pipe_fd(cmd_block);
	waitpid_wrapper(pid, &status, 0);
	return (WEXITSTATUS(status));
}

// choice read pipe
int	*crp(int i, int pipe_a[2], int pipe_b[2])
{
	if (i % 2 == 0)
		return (pipe_b);
	return (pipe_a);
}

// choice write pipe
int	*cwp(int i, int pipe_a[2], int pipe_b[2])
{
	if (i % 2 == 0)
		return (pipe_a);
	return (pipe_b);
}

int	wait_pids(int cmd_cnt, int pids[1000])
{
	int	i;
	int	status;

	i = 0;
	while (i < cmd_cnt)
	{
		waitpid_wrapper(pids[i], &status, 0);
		i ++;
	}
	return (WEXITSTATUS(status));
}

// cmd_list->nextがnullになるまでループ
// 最初の一つ目はhandle_first_blockに
// 真ん中はhandle_middle_blockに
// 最後の一つはhandle_last_blockに
int	exec_command_line(
		t_environ *env, t_list *cmd_list, char **envp, int cmd_cnt)
{
	int			pids[1000];
	t_cmd_block	*cmd_block;
	int			pipe_a[2];
	int			pipe_b[2];
	int			i;

	handle_heredoc_input(env, cmd_list);
	cmd_block = (t_cmd_block *)cmd_list->content;
	if (cmd_cnt == 1)
		return (handle_single_block(cmd_block, envp));
	pids[0] = handle_first_block(cmd_block, envp, pipe_a);
	if (cmd_list->next)
		cmd_list = cmd_list->next;
	i = 1;
	while (cmd_list->next)
	{
		cmd_block = (t_cmd_block *)cmd_list->content;
		pids[i] = handle_middle_block(cmd_block, envp,
				crp(i, pipe_a, pipe_b), cwp(i, pipe_a, pipe_b));
		cmd_list = cmd_list->next;
		i++ ;
	}
	pids[i] = handle_last_block(cmd_list->content, envp,
			crp(i, pipe_a, pipe_b));
	return (wait_pids(cmd_cnt, pids));
}
