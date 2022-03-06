#include "minishell.h"

// 出力のdupをする
// hanlde_cmd_lineで一つ目のcmd_blockを実行する
// dupしてからコマンド実行
int	handle_first_block(t_cmd_block *cmd_block, char	**envp, int pipe_write[2])
{
	int	pid;

	pipe(pipe_write);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_write[0]);
		dup2(pipe_write[1], 1);
		close(pipe_write[1]);
		handle_redirects(cmd_block);
		exec_command(cmd_block->command, cmd_block->args, envp);
	}
	close_doc_pipe_fd(cmd_block);
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

	pipe(pipe_write);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_read[1]);
		close(pipe_write[0]);
		dup2(pipe_read[0], 0);
		dup2(pipe_write[1], 1);
		close(pipe_read[0]);
		close(pipe_write[1]);
		handle_redirects(cmd_block);
		exec_command(cmd_block->command, cmd_block->args, envp);
	}
	close(pipe_read[0]);
	close(pipe_read[1]);
	close_doc_pipe_fd(cmd_block);
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

	pid = fork();
	if (pid == 0)
	{
		close(pipe_read[1]);
		dup2(pipe_read[0], 0);
		close(pipe_read[0]);
		handle_redirects(cmd_block);
		exec_command(cmd_block->command, cmd_block->args, envp);
	}
	close(pipe_read[0]);
	close(pipe_read[1]);
	close_doc_pipe_fd(cmd_block);
	return (pid);
}
