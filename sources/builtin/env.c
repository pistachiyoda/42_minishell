#include "minishell.h"

void	ft_env(t_cmd_block *cmd_block, t_environ *env)
{
	if (cmd_block->args[1] != NULL)
	{
		print_error(cmd_block->args[0], EMESS_INVALID_ARGS);
		return ;
	}
	env = env->next;
	while (env->key != NULL)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}
