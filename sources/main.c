#include "minishell.h"

int	main(void)
{
	char	*prompt;
	char	*str;

	while (1)
	{
		prompt = NULL;
		str = readline(prompt);
		ft_putstr_fd(str, 0);
	}
}
