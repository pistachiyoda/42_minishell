#include "minishell.h"

void	handle_command_line(t_cmd_block *cmd_block, char **envp)
{
	int	pipe_a[FD_MAX][2];

	if (handle_redirect(cmd_block, pipe_a) != 0)
		exit(1);
	exec_command(cmd_block->command, cmd_block->args, envp);
	exit(1);
}

void	handle_single_block(t_cmd_block *cmd_block, char ** envp)
{
	int pid;
	int	status;

	pid = fork();

	if (pid == 0)
		handle_command_line(cmd_block, envp);

	waitpid(pid, &status, 0);
}

// cmd_list->nextがnullになるまでループ
// 最初の一つ目はhandle_first_blockに
// 真ん中はhandle_middle_blockに
// 最後の一つはhandle_last_blockに
int	exec_command_line(t_list *cmd_list, char **envp)
{
	int			pids[1000];
	t_cmd_block *cmd_block;
	int			pipe_a[2];
	int			pipe_b[2];
	int			cmd_cnt;
	int			i;
	int			status;

	cmd_block = (t_cmd_block *)cmd_list->content;
	cmd_cnt = ft_lstsize(cmd_list);
	if (cmd_cnt == 1)
	{
		handle_single_block(cmd_block, envp);
		return (0);
	}
	pipe(pipe_a);
	pids[0] = handle_first_block(cmd_block, envp, pipe_a);
	if (cmd_list->next)
		cmd_list = cmd_list->next;
	i = 1;
	while (1)
	{
		if (!cmd_list->next) // middle処理をせずlast処理をする
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
	return (1);
}
