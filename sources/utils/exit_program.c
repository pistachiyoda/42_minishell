#include "minishell.h"

void	exit_program(int status)
{
	ft_putstr_fd("exit\n", 2);
	exit(status);
}
