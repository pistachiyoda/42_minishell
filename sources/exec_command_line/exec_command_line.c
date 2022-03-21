#include "minishell.h"

int	handle_single_block(t_cmd_block *cmd_block, char **envp)
{
	int			pid;
	int			status;
	bool		sigint;

	sigint = false;
	set_signal(SIG_IGN, SIG_IGN);
	pid = fork_wrapper();
	if (pid == 0)
	{
		set_signal(SIG_DFL, SIG_DFL);
		status = handle_redirects(cmd_block);
		if (status != 0)
			exit(status);
		exec_command(cmd_block, envp);
	}
	close_doc_pipe_fd(cmd_block);
	waitpid_wrapper(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		sigint = true;
	return (get_child_status(status, sigint));
}

void	handle_multi_block(int *pids, t_list *cmd_list, char **envp)
{
	t_cmd_block	*cmd_block;
	t_pipes		*pipes;
	int			i;

	cmd_block = (t_cmd_block *)cmd_list->content;
	pipes = ft_xmalloc(sizeof(t_pipes *) * 2, "exec_command_line");
	pids[0] = handle_first_block(cmd_block, envp, pipes->pipe_a);
	if (cmd_list->next)
		cmd_list = cmd_list->next;
	i = 1;
	while (cmd_list->next)
	{
		cmd_block = (t_cmd_block *)cmd_list->content;
		pids[i] = handle_middle_block(cmd_block, envp,
				crp(i, pipes->pipe_a, pipes->pipe_b),
				cwp(i, pipes->pipe_a, pipes->pipe_b));
		cmd_list = cmd_list->next;
		i++ ;
	}
	pids[i] = handle_last_block(cmd_list->content, envp,
			crp(i, pipes->pipe_a, pipes->pipe_b));
	free(pipes);
}

// cmd_list->nextがnullになるまでループ
// 最初の一つ目はhandle_first_blockに
// 真ん中はhandle_middle_blockに
// 最後の一つはhandle_last_blockに
int	exec_command_line(
		t_environ *env, t_list *cmd_list, char **envp, int cmd_cnt)
{
	int			*pids;
	int			status;

	pids = ft_xmalloc(sizeof(int) * cmd_cnt, "exec_command_line");
	status = handle_heredoc_input(env, cmd_list);
	if (status > 128)
		return (1);
	if (cmd_cnt == 1)
		return (handle_single_block((t_cmd_block *)cmd_list->content, envp));
	else
		handle_multi_block(pids, cmd_list, envp);
	status = wait_pids(cmd_cnt, pids);
	free(pids);
	return (status);
}
