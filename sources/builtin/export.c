#include "minishell.h"

void	display_sorted_environ(t_environ *env)
{
	t_environ	*envlst;
	t_environ	*target;

	envlst = env->next;
	target = envlst;
	while (envlst->key != NULL)
	{
		if (ft_strncmp(target->key, envlst->key, sizeof(target->key)) > 0)
			target = envlst;
		envlst = envlst->next;
	}
	if (target->value == NULL)
		printf("declare -x %s\n", target->key);
	else
		printf("declare -x %s=\"%s\"\n", target->key, target->value);
	target->prev->next = target->next;
	target->next->prev = target->prev;
	if (envlst->next->key == NULL)
		return ;
	display_sorted_environ(envlst);
}

void	ft_export(t_cmd_block *cmd_block, t_environ *env)
{
	int			i;
	char		**split_ele;
	bool		registered;

	i = 1;
	registered = false;
	if (cmd_block->args[i] == NULL)
	{
		display_sorted_environ(env);
		return ;
	}
	while (cmd_block->args[i] != NULL)
	{
		split_ele = split_by_delimiter(cmd_block->args[i], "export");
		env = env->next;
		while (env->key != NULL)
		{
			if (env->key == split_ele[0])
			{
				env->value = split_ele[1];
				registered = true;
			}
			env = env->next;
		}
		if (!registered)
		{
			env = add_environ(env->prev, env, split_ele, "export");
			env = env->next;
		}
		i++;
	}
}
