#include "minishell.h"

// fileが存在しているか、読み取り権限があるかを確認する
bool	is_readable(char *file)
{
	if (access(file, F_OK) == -1)
	{
		printf("no such file or directory");
		return (false);
	}
	if (access(file, R_OK) == -1)
	{
		printf("permission denied");
		return (false);
	}
	return (true);
}

// ファイルを読み込み、書き込みができる状態で開く
int	open_file(char *file)
{
	int	file_fd;

	file_fd = open(file, O_RDWR);
	if (file_fd == -1)
		return (-1); //exit処理
	return (file_fd);
}

// <の処理
void	handle_input(char *filename)
{
	int fd;

	if (!(is_readable(filename)))
		exit(1);
	fd = open_file(filename);
	if (dup2(fd, 0) == -1)
		printf("dup2()"); // exit処理
}

// fileが存在する場合、書き込み権限があるかを確認する(fileが存在しない場合はopen_or_create_fileで新規作成する)
bool	is_writable(char *file)
{
	if (access(file, F_OK) == 0)
		if (access(file, W_OK) == -1)
			return (false);
	return (true);
}

// ファイルを読み込み、書き込みができる状態で開く。ファイルが存在しない場合はopen_flagに従った権限を与えた状態でファイルを新規作成する。
int	open_or_create_file(char *file, int open_flag)
{
	int	file_fd;

	file_fd = open(file, open_flag,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (file_fd == -1)
		return (-1);
	return (file_fd);
}

// >, >>の処理
void	handle_output(char *filename, enum e_REDIRECTS	redirect)
{
	int fd;

	if (!(is_writable(filename)))
		exit(0);
	if (redirect == WRITE)
		fd = open_or_create_file(filename, O_WRONLY | O_CREAT | O_TRUNC);
	if (redirect == APPEND)
		fd = open_or_create_file(filename, O_APPEND | O_WRONLY | O_CREAT);
	if (fd == -1)
		exit(1);
	if (dup2(fd, 1) == -1)
		exit(1);
}

// char	*handle_file_input(char *filename)
// {
// 	int		fd;
// 	char	buf[100];
// 	char	*ret;

// 	fd = open(filename);
// 	ret = ft_strdup("");
// 	while (read(fd, buf, 100))
// 	{
// 		ret = ft_strjoin(ret, buf);
// 	}
// 	return (ret);
// }

// char	*handle_heredoc(char *eof)
// {
// 	return ft_strdup("");
// }

int		handle_input_redirect(t_cmd_block *cmd_block)
{
	t_list		*redirect_node;
	t_redirects	*redirect;
	// char		*input;

	redirect_node = cmd_block->redirects;
	if (!redirect_node)
		return (0);
	// input = NULL;
	while (1)
	{
		// free(input);
		redirect = redirect_node->content;
		if (redirect->redirect == INPUT)
			handle_input(redirect->target);
		if (redirect->redirect == WRITE || redirect->redirect == APPEND)
			handle_output(redirect->target, redirect->redirect);
		if (redirect_node->next == NULL)
			break;
		redirect_node = redirect_node->next;
	}
	// inputをパイプに書き込む処理
	return (0);
}

int	exec_command_line(t_list *cmd_list, char **envp)
{
	int	pid;
	int	status;
	// int	pipe_fds[2];

	// if (pipe(pipe_fds) == -1)
	// {
	// 	perror("pipe()");
	// 	exit(1);
	// }

	pid = fork();
	if (pid == 0)
	{
		// if (close(pipe_fds[1]) == -1)
		// 	exit(1);
		handle_input_redirect((t_cmd_block *)cmd_list->content);
		exec_command(((t_cmd_block *)cmd_list->content)->command,
			((t_cmd_block *)cmd_list->content)->args, envp);
	}
	waitpid(pid, &status, 0);
	return (1);
}
