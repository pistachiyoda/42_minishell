#include "minishell.h"

t_environ	*get_tmp_min(t_environ *env, int *min_i, char *flags, int f_len)
{
	int			i;
	t_environ	*min;

	i = 0;
	min = env;
	while (*min_i > i++)
		min = min->next;
	while (flags[*min_i] == '1')
	{
		min = min->next;
		if (*min_i < f_len - 1)
			(*min_i)++;
		else
		{
			min = min->next->next;
			*min_i = 1;
		}
	}
	return (min);
}

void	print_statement(t_environ *min, int min_i, char *flags, int f_len)
{
	int			i;
	t_environ	*head;

	i = 0;
	head = min;
	if (min->value == NULL)
		printf("declare -x %s\n", min->key);
	else
		printf("declare -x %s=\"%s\"\n", min->key, min->value);
	while (head->key != NULL)
		head = head->next;
	while (i < f_len)
	{
		if (flags[i++] == '0')
		{
			display_sorted_env(head->next, min_i, flags, f_len);
			break ;
		}
	}
}

void	display_sorted_env(t_environ *env, int min_i, char *flags, int f_len)
{
	int			i;
	size_t		len;
	t_environ	*min;

	min = env;
	min = get_tmp_min(env, &min_i, flags, f_len);
	len = ft_strlen(min->key);
	flags[min_i] = '1';
	i = 0;
	while (env->key != NULL)
	{
		if ((ft_strncmp(min->key, env->key, len) > 0) && (flags[i] == '0'))
		{
			flags[min_i] = '0';
			min = env;
			min_i = i;
			len = ft_strlen(min->key);
			flags[min_i] = '1';
		}
		env = env->next;
		i++;
	}
	print_statement(min, min_i, flags, f_len);
}

char	*omit_quoat(char *split_ele, char *non_quoat, int *quoat, int *dquoat)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (split_ele[i] != '\0')
	{
		if (split_ele[i] != '"' && split_ele[i] != '\'')
			non_quoat[j++] = split_ele[i];
		else if (split_ele[i] == '\'')
			(*quoat)++;
		else if (split_ele[i] == '"')
			(*dquoat)++;
		i++;
	}
	return (non_quoat);
}

void	ft_export(t_cmd_block *cmd_block, t_environ *env)
{
	int		i;
	char	*flags;
	char	**split_ele;
	bool	registered;
	int		env_num;

	i = 1;
	registered = false;
	if (cmd_block->args[i] == NULL)
	{
		env_num = count_environ_ele(env);
		flags = malloc(sizeof(char) * (env_num + 1));
		while (i < env_num + 1)
			flags[i++] = '0';
		flags[i] = '\0';
		display_sorted_env(env->next, 0, flags, env_num);
		free(flags);
		return ;
	}
	i = 1;
	while (cmd_block->args[i] != NULL)
	{
		split_ele = ft_split(cmd_block->args[i], '=');
		env = env->next;
		while (env->key != NULL)
		{
			if (ft_strncmp(env->key, split_ele[0], ft_strlen(split_ele[0])) == 0)
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
			registered = false;
		}
		i++;
	}
}
