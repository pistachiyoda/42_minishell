#include "minishell.h"

void	sigint_handler(int sig)
{
	int	save_errno;

	(void)sig;
	save_errno = errno;
	write(2, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_status = 1;
	errno = save_errno;
}

void	set_signal(void (*func1)(int), void (*func2)(int))
{
	if (signal(SIGINT, func1) == SIG_ERR
		|| signal(SIGQUIT, func2) == SIG_ERR)
	{
		print_error("signal error", strerror(errno));
		ft_putstr_fd("\n", 2);
		exit(EXIT_FAILURE);
	}
}
