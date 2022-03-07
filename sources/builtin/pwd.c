#include "minishell.h"

int	ft_pwd(void)
{
	char	*cwd_path;

	cwd_path = getcwd(NULL, 1024);
	if (cwd_path == NULL)
	{
		perror("getcwd()");
		return (1);
	}
	printf("%s\n", cwd_path);
	return (0);
}
