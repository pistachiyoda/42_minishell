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
		if (ft_strlen(env->key) > len)
			len = ft_strlen(env->key);
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
