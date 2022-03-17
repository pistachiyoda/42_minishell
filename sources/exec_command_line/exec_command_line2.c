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

int	get_child_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(2, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit: 3", 2);
		return (WTERMSIG(status) + 128);
	}
	return (1);
}

int	wait_pids(int cmd_cnt, int pids[1000])
{
	int	i;
	int	status;

	i = 0;
	while (i < cmd_cnt)
	{
		waitpid_wrapper(pids[i], &status, 0);
		i ++;
	}
	return (get_child_status(status));
}
