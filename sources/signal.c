#include "minishell.h"

volatile sig_atomic_t	received_signal;

void	handler(int sig)
{
	(void)sig;
	rl_replace_line("\n", 1); //0?
	rl_clear_history();
	rl_redisplay();
}

void	set_sigaction(void)
{
	received_signal = 0;
	struct sigaction	act;
	struct sigaction	act2;

	act.sa_handler = handler;
	act2.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act2, NULL);
}

void	wait_signal(void)
{
	if (received_signal == SIGINT)
	{
		printf("SIGINT\n");
	}
}
