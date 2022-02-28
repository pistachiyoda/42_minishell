#include "minishell.h"

// bool	is_registered(t_environ *env, char **split_ele, bool key_only)
// {
// 	size_t	len;
// 	bool	registered;

// 	len = ft_strlen(split_ele[0]);
// 	registered = false;
// 	env = env->next;
// 	while (env->key != NULL)
// 	{
// 		if (ft_strlen(env->key) > len)
// 			len = ft_strlen(env->key);
// 		if (ft_strncmp(env->key, split_ele[0], len) == 0)
// 		{
// 			if (!key_only)
// 				env->value = split_ele[1];
// 			registered = true;
// 		}
// 		env = env->next;
// 	}
// 	return (registered);
// }

char	*is_registered(t_environ *env, char **split_ele, bool key_only)
{
	size_t	len;

	len = ft_strlen(split_ele[0]);
	env = env->next;
	while (env->key != NULL)
	{
		if (ft_strlen(env->key) > len)
			len = ft_strlen(env->key);
		if (ft_strncmp(env->key, split_ele[0], len) == 0)
		{
			if (!key_only)
				env->value = split_ele[1];
			return (env->value);
		}
		env = env->next;
	}
	return (NULL);
}

int	update_environ(t_cmd_block *cmd_block, t_environ *env, int i)
{
	char	**split_ele;
	bool	key_only;

	key_only = false;
	if (ft_strchr(cmd_block->args[i], '=') == NULL)
		key_only = true;
	split_ele = ft_split(cmd_block->args[i], '=');
	malloc_check(split_ele, "export");
	if (ft_isdigit(split_ele[0][0]) == 1) //valにquoteが文字としてあるパターンも同じエラー
	{
		free_2d_array(split_ele);
		return (-1);
	}
	if (is_registered(env, split_ele, key_only) == NULL)
	{
		env = add_environ(env->prev, env, split_ele, "export");
		env = env->next;
	}
	return (++i);
}

void	ft_export(t_cmd_block *cmd_block, t_environ *env)
{
	int		i;
	char	*flags;
	int		env_num;

	if (cmd_block->args[1] == NULL)
	{
		env_num = count_environ_ele(env);
		flags = xmalloc(sizeof(char) * (env_num + 1), "export");
		flags = ft_memset(flags, '0', env_num);
		flags[env_num + 1] = '\0';
		display_sorted_env(env->next, 0, flags, env_num);
		free(flags);
		return ;
	}
	i = 1;
	while (cmd_block->args[i] != NULL)
	{
		i = update_environ(cmd_block, env, i);
		if (i == -1)
		{
			print_error("export", EMESS_INVALID_ID);
			return ;
		}
	}
}
