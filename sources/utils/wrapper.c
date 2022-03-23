/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 22:11:48 by fmai              #+#    #+#             */
/*   Updated: 2022/03/09 22:11:48 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_wrapper(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe()");
		exit(1);
	}
}

int	fork_wrapper(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork()");
		exit(1);
	}
	return (pid);
}

void	close_wrapper(int fd)
{
	if (close(fd) == -1)
	{
		perror("close()");
		exit(1);
	}
}

void	waitpid_wrapper(pid_t pid, int *wstatus, int options)
{
	if (waitpid(pid, wstatus, options) == -1)
	{
		perror("waitpid()");
		exit(1);
	}
}

char	**ft_split_wrapper(char const *s, char c)
{
	char	**ret;

	ret = ft_split(s, c);
	if (ret == NULL)
	{
		perror("ft_split()");
		exit(1);
	}
	return (ret);
}
