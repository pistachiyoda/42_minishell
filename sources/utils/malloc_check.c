#include "minishell.h"

t_list	*xlstnew(void *content, char *target)
{
	t_list	*lst;

	lst = ft_lstnew(content);
	if (lst == NULL)
	{
		print_error(target, EMESS_MALLOC_FAIL);
		exit(EXIT_FAILURE);
	}
	return (lst);
}

char	*xstrdup(const char *src, char *target)
{
	char	*ret;

	ret = ft_strdup(src);
	if (ret == NULL)
	{
		print_error(target, EMESS_MALLOC_FAIL);
		exit(EXIT_FAILURE);
	}
	return (ret);
}

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
