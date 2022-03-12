#include "minishell.h"

char	*expand_env_variables(t_environ *env, char *key)
{
	char *ret;

	ret = get_env_val(key, t_environ_to_vector(env));
	free(key);
	if (ret == NULL)
		return (ft_xstrdup("", "expand_env_variables_in_buf"));
	return (ret);
}

char	*get_key(char *buf, int *i)
{
	int		cnt;
	char	*key;
	int		j;

	cnt = 0;
	(*i) ++;
	while (buf[*i] != '$' && buf[*i] != ' ' && buf[*i] != '\n' && buf[*i] != '\0')
	{
		cnt ++;
		(*i) ++;
	}
	key = ft_xmalloc(sizeof(char *) * (cnt + 1), "get_key");
	(*i) -= cnt;
	j = 0;
	while (j < cnt)
	{
		key[j] = buf[*i];
		j ++;
		(*i) ++;
	}
	key[j] = '\0';
	(*i) --;
	return (key);
}

char	*expand_env_variables_in_buf(t_environ *env, char *buf)
{
	int		i;
	char	*tmp_str;
	char	*value;
	char	*joined_str;
	
	i = 0;
	joined_str = ft_xstrdup("", "expand_env_variables_in_buf");
	while (buf[i])
	{
		if (buf[i] == '$')
		{
			value = expand_env_variables(env, get_key(buf, &i));
			joined_str = ft_xstrjoin_with_free(joined_str, value, "expand_env_variables_in_buf");
		}
		else
		{		
			tmp_str = ft_xmalloc(sizeof(char *) * 2, "expand_env_variables_in_buf");
			tmp_str[0] = buf[i];
			tmp_str[1] = '\0';
			joined_str = ft_xstrjoin_with_free(joined_str, tmp_str, "expand_env_variables_in_buf");
		}
		i++;
	}
	return (joined_str);
}
