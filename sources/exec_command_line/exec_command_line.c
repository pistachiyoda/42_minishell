#include "minishell.h"

int	handle_single_block(t_cmd_block *cmd_block, char **envp)
{
	int	pid;
	int	status;
	int doc_pipe_fds_arr[FD_MAX][2];
	t_list	*redirect_node;
	t_redirects	*redirect;
	int	ret;

	if (handle_heredoc_input(cmd_block, doc_pipe_fds_arr) != 0)
		exit(1);
	pid = fork();
	if (pid == 0)
	{
		redirect_node = cmd_block->redirects;
		while (1)
		{
			if (!redirect_node)
				break ;
			redirect = redirect_node->content;
			ret = handle_redirect(redirect, cmd_block, doc_pipe_fds_arr);
			if (ret != 0)
				exit(ret);
			if (redirect_node->next == NULL)
				break ;
			redirect_node = redirect_node->next;
		}
		exec_command(cmd_block->command, cmd_block->args, envp);
	}
	close_doc_pipe_fds(doc_pipe_fds_arr, cmd_block);
	waitpid(pid, &status, 0);
	return (0);
}

// cmd_list->nextがnullになるまでループ
// 最初の一つ目はhandle_first_blockに
// 真ん中はhandle_middle_blockに
// 最後の一つはhandle_last_blockに
int	exec_command_line(t_list *cmd_list, char **envp)
{
	int			pids[1000];
	t_cmd_block	*cmd_block;
	int			pipe_a[2];
	int			pipe_b[2];
	int			cmd_cnt;
	int			i;
	int			status;

	cmd_block = (t_cmd_block *)cmd_list->content;
	cmd_cnt = ft_lstsize(cmd_list);
	if (cmd_cnt == 1)
		return handle_single_block(cmd_block, envp);
	pipe(pipe_a);
	pids[0] = handle_first_block(cmd_block, envp, pipe_a);
	if (cmd_list->next)
		cmd_list = cmd_list->next;
	i = 1;
	while (1)
	{
		if (!cmd_list->next)
			break ;
		cmd_block = (t_cmd_block *)cmd_list->content;
		if (i % 2 != 0)
		{
			pipe(pipe_b);
			pids[i] = handle_middle_block(cmd_block, envp, pipe_a, pipe_b);
		}
		if (i % 2 == 0)
		{
			pipe(pipe_a);
			pids[i] = handle_middle_block(cmd_block, envp, pipe_b, pipe_a);
		}
		cmd_list = cmd_list->next;
		i++ ;
	}
	if (i % 2 != 0)
		pids[i] = handle_last_block(cmd_list->content, envp, pipe_a);
	if (i % 2 == 0)
		pids[i] = handle_last_block(cmd_list->content, envp, pipe_b);
	i = 0;
	while (i < cmd_cnt)
	{
		waitpid(pids[i], &status, 0);
		i ++;
	}
	return (0);
}
