#include "minishell.h"

char	**set_data(char	*str, int key_cnt, int val_cnt)
{
	int 	str_i;
	int		key_i;
	int		val_i;
	char	**data;

	data = ft_xmalloc(sizeof(char *) * 3, "get_env_val");
	data[0] = ft_xmalloc(sizeof(char) * (key_cnt + 1), "get_env_val");
	data[1] = ft_xmalloc(sizeof(char) * (val_cnt + 1), "get_env_val");
	data[2] = NULL;
	str_i = 0;
	key_i = 0;
	while (str[str_i] && key_cnt > key_i)
	{
		data[0][key_i] = str[str_i];
		key_i ++;
		str_i ++;
	}
	data[0][key_i] = '\0';
	str_i ++;
	val_i = 0;
	while (str[str_i] && val_cnt > val_i)
	{
		data[1][val_i] = str[str_i];
		val_i ++;
		str_i ++;
	}
	data[1][val_i] = '\0';
	return (data);
}

char	**split_into_two(char *str, char splitter)
{
	int		i;
	int		key_cnt;
	int		val_cnt;
	bool	is_key;

	i = 0;
	key_cnt = 0;
	val_cnt = 0;
	is_key = true;
	while (str[i])
	{
		if (is_key && str[i] == splitter)
		{
			is_key = false;
			i ++;
			continue;
		}
		if (is_key)
			key_cnt ++;
		else
			val_cnt ++;
		i ++;
	}
	return (set_data(str, key_cnt, val_cnt));
}

// 最初の=で二つに分割する
char	*get_env_val(char *key, char **envp)
{
	int		i;
	char	**data;
	char	*env_val;

	i = 0;
	while (envp[i] != NULL)
	{
		data = split_into_two(envp[i], '=');
		if (ft_strlen(data[0]) == ft_strlen(key)
			&& ft_strncmp(envp[i], key, ft_strlen(key)) == 0)
			{
				printf("Key : %s\nVal : %s\n", key, data[1]);
				env_val = ft_xstrdup(data[1], "get_env_val"); // free
				free_2d_array(data);
				return (env_val);
			}
		i++;
	}
	return (NULL);
}
