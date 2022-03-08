#include "minishell.h"

char	*xstrjoin(char const *s1, char const *s2, char *target)
{
	char	*ret;

	ret = ft_strjoin(s1, s2);
	if (ret == NULL)
	{
		print_error(target, EMESS_MALLOC_FAIL);
		exit(EXIT_FAILURE);
	}
	return (ret);
}
