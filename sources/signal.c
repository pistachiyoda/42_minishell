#include "minishell.h"

int	received_signal;

void	handler(int sig, siginfo_t *info, void *q)
{
	(void)info;
	(void)q;
	received_signal = sig;
}

void	set_sigaction(void)
{
	struct sigaction	act;
	// siginfo_t			siginfo;

	act.sa_sigaction = handler;
	act.sa_flags = SA_SIGINFO;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);
}

int	wait_signal(void)
{
	received_signal = 0;
	set_sigaction();
	while (1)
	{
		if (received_signal == SIGINT)
		{
			printf("SIGINT\n");
		}
		else if (received_signal == SIGQUIT)
			return (SIGQUIT);
		else if (received_signal == SIGKILL)
			printf("ctrl -D\n");
		pause();
	}
}
