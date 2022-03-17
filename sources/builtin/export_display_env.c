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

void	print_env(t_environ *min)
{
	int	i;

	i = 0;
	if (min->value == NULL)
		printf("declare -x %s\n", min->key);
	else
	{
		printf("declare -x %s=\"", min->key);
		while (min->value[i] != '\0')
		{
			if (ft_strchr("\"$`\n\\", min->value[i]))
				printf("\\%c", min->value[i]);
			else
				printf("%c", min->value[i]);
			i++;
		}
		printf("\"\n");
	}
}

void	display_min_env(t_environ *min, int min_i, char *flags, int f_len)
{
	int			i;
	t_environ	*head;

	i = 0;
	head = min;
	print_env(min);
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

size_t	get_len(char *str1, char *str2)
{
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(str1);
	len2 = ft_strlen(str2);
	if (len1 >= len2)
		return (len1);
	else
		return (len2);
}

void	display_sorted_env(t_environ *env, int min_i, char *flags, int f_len)
{
	int			i;
	t_environ	*min;

	min = env;
	min = get_tmp_min(env, &min_i, flags, f_len);
	flags[min_i] = '1';
	i = 0;
	while (env->key != NULL)
	{
		if (ft_strncmp(min->key, env->key, get_len(min->key, env->key)) > 0
			&& flags[i] == '0')
		{
			flags[min_i] = '0';
			min = env;
			min_i = i;
			flags[min_i] = '1';
		}
		env = env->next;
		i++;
	}
	display_min_env(min, min_i, flags, f_len);
}
