#include "minishell.h"

int	ft_cd(t_cmd_block *cmd_block, t_environ *env)
{
	char	*path;
	char	**envp;
	int		ret;
	char	*e_mes;

	if (cmd_block->args[1])
		path = cmd_block->args[1];
	else
	{
		envp = t_environ_to_vector(env);
		path = get_env_val("HOME", envp);
	}
	ret = chdir(path);
	if (ret == -1)
	{
		e_mes = ft_strjoin_wrapper("minishell: cd: ", path);
		perror(e_mes);
		free(e_mes);
		return (1);
	}
	return (0);
}
