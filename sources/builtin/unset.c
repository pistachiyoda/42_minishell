#include "minishell.h"

void	ft_unset(t_cmd_block *cmd_block, t_environ *env)
{
	int			i;
	int			len;
	t_environ	*tmp;

	i = 1;
	while (cmd_block->args[i] != NULL)
	{
		env = env->next;
		len = ft_strlen(cmd_block->args[i]);
		while (env->key != NULL)
		{
			if (ft_strncmp(env->key, cmd_block->args[i], len) == 0)
			{
				env->prev->next = env->next;
				env->next->prev = env->prev;
				tmp = env;
				env = env->prev;
				free(tmp->key);
				free(tmp->value);
				free(tmp);
			}
			env = env->next;
		}
		i++;
	}
}
