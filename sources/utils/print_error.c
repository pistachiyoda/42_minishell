#include "minishell.h"

void	print_error(char *target, char *message)
{
	ft_putstr_fd(target, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(message, 2);
}
