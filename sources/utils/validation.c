#include "minishell.h"

bool	is_valid_arg(char *str)
{
	int	j;

	j = 0;
	while (str[j] != '\0')
	{
		if (j == 0 && ft_isalpha(str[0]) != 1 && str[0] != '_')
			break ;
		if (j != 0 && ft_isalnum(str[j]) != 1 && str[j] != '_')
			break ;
		j++;
	}
	if (str[j] != '\0')
	{
		print_error(str, EMESS_INVALID_ID);
		g_status = 1;
		return (false);
	}
	return (true);
}
