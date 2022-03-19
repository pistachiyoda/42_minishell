#include "minishell.h"

int	ft_pwd(t_environ *env)
{
	char	*cwd_path;

	cwd_path = get_env_val("PWD", t_environ_to_vector(env));
	if (cwd_path == NULL)
	{
		cwd_path = getcwd(NULL, 1024);
		if (cwd_path == NULL)
		{
			perror("getcwd()");
			return (1);
		}
	}
	printf("%s\n", cwd_path);
	return (0);
}
