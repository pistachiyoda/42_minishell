#include "minishell.h"

void	exit_program(int status)
{
	ft_putendl_fd("exit", 2);
	exit(status);
}
