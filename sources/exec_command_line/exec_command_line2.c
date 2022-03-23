/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_line2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 23:03:08 by fmai              #+#    #+#             */
/*   Updated: 2022/03/21 23:03:08 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	get_child_status(int status, bool sigint)
{
	int	res;

	if (WIFEXITED(status))
		res = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit: 3", 2);
		res = WTERMSIG(status) + 128;
	}
	if (sigint)
		ft_putstr_fd("\n", 2);
	return (res);
}

int	wait_pids(int cmd_cnt, int pids[1000])
{
	int		i;
	int		status;
	bool	sigint;

	i = 0;
	sigint = false;
	while (i < cmd_cnt)
	{
		waitpid_wrapper(pids[i], &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			sigint = true;
		i ++;
	}
	return (get_child_status(status, sigint));
}
