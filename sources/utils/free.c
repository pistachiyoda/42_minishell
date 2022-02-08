#include "minishell.h"

void	free_2d_array(char **two_d_array)
{
	int	i;

	i = 0;
	while (two_d_array[i] != NULL)
	{
		free(two_d_array[i]);
		i++;
	}
	free(two_d_array);
}
