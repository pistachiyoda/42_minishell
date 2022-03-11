#include "minishell.h"

int	check_str_type(char *str)
{
	int	i;
	int	status;
	int	count;

	i = 0;
	status = SPACELESS;
	count = 0;
	while (str[i] != '\0')
	{
		if (is_space_tab_newline(str[i]))
		{
			status = SPACE;
			count++;
		}
		i++;
	}
	if (i != 0 && count == i)
		status = ALL_SPACE;
	return (status);
}
