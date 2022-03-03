#include "minishell.h"

// 出力のdupをする
// hanlde_cmd_lineで一つ目のcmd_blockを実行する
// dupしてからコマンド実行
int	handle_first_block(t_cmd_block *cmd_block, char	**envp, int pipe_write[2])
{
	int	pid;
	int doc_pipe_fds_arr[FD_MAX][2];
	t_list	*redirect_node;
	t_redirects	*redirect;
	int	ret;

	if (handle_heredoc_input(cmd_block, doc_pipe_fds_arr) != 0)
		exit(1);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_write[0]);
		dup2(pipe_write[1], 1);
		close(pipe_write[1]);
		redirect_node = cmd_block->redirects;
		while (1)
		{
			if (!redirect_node)
				break ;
			redirect = redirect_node->content;
			ret = handle_redirect(redirect, cmd_block, doc_pipe_fds_arr);
			if (ret != 0)
				return (ret);
			if (redirect_node->next == NULL)
				break ;
			redirect_node = redirect_node->next;
		}
		exec_command(cmd_block->command, cmd_block->args, envp);
	}
	close_doc_pipe_fds(doc_pipe_fds_arr, cmd_block);
	return (pid);
}

// 入力と出力のdupをする
// 奇数の時は、aから読み込んでbに書き込む
// 偶数の時は、bから読み込んでaに書き込む
// hanlde_cmd_lineで２つ目のcmd_blockを実行する
// dupしてからコマンド実行
int	handle_middle_block(
	t_cmd_block *cmd_block, char **envp, int pipe_read[2], int pipe_write[2])
{
	int	pid;
	int doc_pipe_fds_arr[FD_MAX][2];
	t_list	*redirect_node;
	t_redirects	*redirect;
	int	ret;

	if (handle_heredoc_input(cmd_block, doc_pipe_fds_arr) != 0)
		exit(1);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_read[1]);
		close(pipe_write[0]);
		dup2(pipe_read[0], 0);
		dup2(pipe_write[1], 1);
		close(pipe_read[0]);
		close(pipe_write[1]);
		redirect_node = cmd_block->redirects;
		while (1)
		{
			if (!redirect_node)
				break ;
			redirect = redirect_node->content;
			ret = handle_redirect(redirect, cmd_block, doc_pipe_fds_arr);
			if (ret != 0)
				return (ret);
			if (redirect_node->next == NULL)
				break ;
			redirect_node = redirect_node->next;
		}
		exec_command(cmd_block->command, cmd_block->args, envp);
	}
	close(pipe_read[0]);
	close(pipe_read[1]);
	close_doc_pipe_fds(doc_pipe_fds_arr, cmd_block);
	return (pid);
}

// 入力のdupをする
// 奇数の時はaから読み込む
// 偶数の時はbから読み込む
// hanlde_cmd_lineで２つ目のcmd_blockを実行する
// dupしてからコマンド実行
int	handle_last_block(t_cmd_block *cmd_block, char	**envp, int pipe_read[2])
{
	int	pid;
	int doc_pipe_fds_arr[FD_MAX][2];
	t_list	*redirect_node;
	t_redirects	*redirect;
	int	ret;
	if (handle_heredoc_input(cmd_block, doc_pipe_fds_arr) != 0)
		exit(1);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_read[1]);
		dup2(pipe_read[0], 0);//
		close(pipe_read[0]);
		redirect_node = cmd_block->redirects;
		while (1)
		{
			if (!redirect_node)
				break ;
			redirect = redirect_node->content;
			ret = handle_redirect(redirect, cmd_block, doc_pipe_fds_arr);
			if (ret != 0)
				return (ret);
			if (redirect_node->next == NULL)
				break ;
			redirect_node = redirect_node->next;
		}
		exec_command(cmd_block->command, cmd_block->args, envp);
	}
	close(pipe_read[0]);
	close(pipe_read[1]);
	close_doc_pipe_fds(doc_pipe_fds_arr, cmd_block);
	return (pid);
}
