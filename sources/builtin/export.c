#include "minishell.h"

void	update_environ(char *str, t_environ *env)
{
	char	**split_ele;
	bool	key_only;

	key_only = false;
	split_ele = split_by_delimiter(str, &key_only, "export");
	if (is_valid_arg(split_ele[0])
		&& is_env_registerd(env, split_ele, key_only, "export") == NULL)
	{
		env = add_environ(env->prev, env, split_ele, "export");
		env = env->next;
		free(split_ele);
	}
	else
		free_2d_array(split_ele);
}

void	ft_export(t_cmd_block *cmd_block, t_environ *env)
{
	int		i;
	char	*flags;
	int		env_num;

	g_status = 0;
	if (cmd_block->args[1] == NULL)
	{
		env_num = count_environ_ele(env);
		flags = xmalloc(sizeof(char) * (env_num + 1), "export");
		flags = ft_memset(flags, '0', env_num);
		flags[env_num] = '\0';
		display_sorted_env(env->next, 0, flags, env_num);
		free(flags);
		return ;
	}
	i = 1;
	while (cmd_block->args[i] != NULL)
	{
		update_environ(cmd_block->args[i], env);
		i++;
	}
}
