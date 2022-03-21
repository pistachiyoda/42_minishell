#include "minishell.h"

bool	is_readable(char *file)
{
	if (access(file, R_OK) == -1)
		return (false);
	return (true);
}

int	open_file(char *file)
{
	int	file_fd;

	file_fd = open(file, O_RDONLY);
	if (file_fd == -1)
	{
		perror("open()");
		exit(1);
	}
	return (file_fd);
}

bool	is_writable(char *file)
{
	if (access(file, F_OK) == 0)
		if (access(file, W_OK) == -1)
			return (false);
	return (true);
}

int	open_or_create_file(char *file, int open_flag)
{
	int	file_fd;

	file_fd = open(file, open_flag,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (file_fd == -1)
	{
		perror("open()");
		exit(1);
	}
	return (file_fd);
}
