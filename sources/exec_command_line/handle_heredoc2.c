#include "minishell.h"

char	*expand_env_variables(t_environ *env, char *key)
{
	char	*ret;
	char	**envp;

	envp = t_environ_to_vector(env);
	ret = get_env_val(key, envp);
	free(key);
	free_2d_array(envp);
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
	(*i)++;
	while (buf[*i] != '$' && buf[*i] != ' '
		&& buf[*i] != '\n' && buf[*i] != '\0')
	{
		cnt ++;
		(*i)++;
	}
	key = ft_xmalloc(sizeof(char *) * (cnt + 1), "get_key");
	(*i) -= cnt;
	j = 0;
	while (j < cnt)
	{
		key[j] = buf[*i];
		j ++;
		(*i)++;
	}
	key[j] = '\0';
	(*i)--;
	return (key);
}

char	*expand_env_variables_in_buf(t_environ *env, char *buf)
{
	int		i;
	char	*tmp_str;
	char	*value;
	char	*joined_str;

	i = 0;
	joined_str = ft_xstrdup("", "expand_env");
	while (buf[i])
	{
		if (buf[i] == '$')
		{
			value = expand_env_variables(env, get_key(buf, &i));
			joined_str = ft_xstrjoin_with_free(joined_str, value, "expand_env");
		}
		else
		{
			tmp_str = ft_xmalloc(sizeof(char *) * 2, "expand_env");
			tmp_str[0] = buf[i];
			tmp_str[1] = '\0';
			joined_str = ft_xstrjoin_with_free(
					joined_str, tmp_str, "expand_env");
		}
		i++;
	}
	return (joined_str);
}

void	handle_unused_heredoc(char *str)
{
	free(str);
	exit(0);
}

void	flush_heredoc(char *str, int doc_pipe_fds[2])
{
	close_wrapper(doc_pipe_fds[0]);
	if (write(doc_pipe_fds[1], str, ft_strlen(str)) == -1)
	{
		perror("write()");
		exit(1);
	}
	close_wrapper(doc_pipe_fds[1]);
	free(str);
	exit(0);
}
