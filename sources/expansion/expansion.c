#include "minishell.h"

int	is_in_quote_dquote(char *str, int i, int status)
{
	if (str[i] == '\'' && status != DQUOTE)
	{
		if (status == NONE)
			status = QUOTE;
		else if (status == QUOTE)
			status = NONE;
	}
	else if (str[i] == '"' && status != QUOTE)
	{
		if (status == NONE)
			status = DQUOTE;
		else if (status == DQUOTE)
			status = NONE;
	}
	return (status);
}
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

char	*param_expansion(t_environ *env, char *str, int *i)
{
	char	*expanded;

	// $OKみたいにvalueがない変数か未登録の変数の場合は、NULLが返ってくる
	expanded = is_registered(env, &str, true);
	if (expanded == NULL)
		return (NULL);
	free(str);
	return (expanded);
}

// dquoteは一つだけじゃなく、複数でも許容される必要あり
// echo """'$TEST'""" ->echo "'le\"mon\"'" -> 'le"mon"'
// ($の前に”が奇数個存在する&&奇数個の最後の一つが'より前方に存在する) || ()
// (status==dquote||status==NONE) && 文字=='$' ->展開する
char	*find_param(t_cmd_block *cmd, t_environ *env, char *str)
{
	int	i;
	int	status;
	char	*save;
	char	*tmp;
	char	*left;

	i = 0;
	status = NONE;
	while (str[i] != '\0')
	{
		status = is_in_quote_dquote(str, i, status);
		if (status != QUOTE && str[i + 1] == '$')
		{
			tmp = xstrdup(save, "expansion");
			tmp[i + 1] = '\0';
			i += 2;
			// left = str[];変数名以降を代入する
			save = ft_strjoin(tmp, param_expansion(env, str, &i));
			free(tmp);
		}
		i++;
	}
	return (str);
}

t_list	*expansion(t_list *tokens, t_environ *env)
{
	t_cmd_block	*cmd;
	t_redirects	*redirects;
	int			i;

	while (tokens != NULL)
	{
		cmd = tokens->content;
		if (cmd != NULL)
		{
			if (cmd->command != NULL)
			{
				find_param(cmd, env, cmd->command);
				i = 0;
				while (cmd->args[i] != NULL)
				{
					find_param(cmd, env, cmd->args[i]);
					i++;
				}
			}
			while (cmd->redirects != NULL)
			{
				redirects = cmd->redirects->content;
				find_param(cmd, env, redirects->target);
				cmd->redirects = cmd->redirects->next;
			}
		}
		tokens = tokens->next;
	}
}
