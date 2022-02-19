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
