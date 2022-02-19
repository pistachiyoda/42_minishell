#include "minishell.h"

char	*xsubstr(char const *s, unsigned int start, size_t len, char *target)
{
	char	*ret;

	ret = ft_substr(s, start, len);
	if (ret == NULL)
	{
		print_error(target, EMESS_MALLOC_FAIL);
		exit(EXIT_FAILURE);
	}
	return (ret);
}

void	malloc_check(void *words, char *target)
{
	if (words == NULL)
	{
		print_error(target, EMESS_MALLOC_FAIL);
		exit(EXIT_FAILURE);
	}
}
