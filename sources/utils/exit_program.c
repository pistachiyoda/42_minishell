#include "minishell.h"

void	exit_program(int status)
{
	ft_putstr_fd("exit", 2);
	exit(status);
}
