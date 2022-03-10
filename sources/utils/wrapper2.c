#include "minishell.h"

char	*ft_strjoin_wrapper(char const *s1, char const *s2)
{
	char	*ret;

	ret = ft_strjoin(s1, s2);
	if (ret == NULL)
	{
		perror("ft_strjoin()");
		exit(1);
	}
	return (ret);
}

void	execve_wrapper(
		const char *pathname, char *const argv[], char *const envp[])
{
	if (execve(pathname, argv, envp) == -1)
	{
		perror("execve()");
		exit(1);
	}
}

void	dup2_wrapper(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1)
	{
		perror("dup2()");
		exit(1);
	}
}
