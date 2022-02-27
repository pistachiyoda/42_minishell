#include "minishell.h"

void	syntax_error(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(EMESS_SYNTAX, 2);
	ft_putstr_fd(" `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'\n", 2);
}

void	print_error(char *target, char *message)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(target, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(message, 2);
}
