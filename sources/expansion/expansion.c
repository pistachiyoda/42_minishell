#include "minishell.h"

int	split_by_space2(char *str, t_list **words, int *i, int start)
{
	t_list	*new;

	if (*words == NULL)
		*words = xlstnew(xsubstr(str, start, *i - start, "lexer"), "lexer");
	else
	{
		new = xlstnew(xsubstr(str, start, *i - start, "lexer"), "lexer");
		ft_lstadd_back(words, new);
	}
	while (str[*i + 1] != '\0' && is_space_tab_newline(str[*i + 1]))
		(*i)++;
	start = *i + 1;
	return (start);
}

size_t	get_left(char *str, int i, char **left)
{
	while (str[i + 1] != '\0')
	{
		if (str[i + 1] == '"' || str[i + 1] == '\'' || str[i + 1] == '$')
		{
			*left = &str[i + 1];
			break ;
		}
		i++;
	}
	if (*left != NULL)
		return (ft_strlen(*left));
	return (0);
}

char	*split_expanded_by_space(t_list **words, int status, char *head, int *splitted)
{
	int		j;
	int		start;
	char	*tmp;

	j = 0;
	start = 0;
	if (status == NONE)
	{
		while (ft_strlen(head) - j > 0)
		{
			if (is_space_tab_newline(head[j]))
			{
				start = split_by_space2(head, words, &j, start);
				*splitted = 1;
			}
			j++;
		}
		if (start != j)
		{
			tmp = xsubstr(head, start, j - start, "expansion");
			free(head);
			head = tmp;
		}
	}
	return (head);
}

char	*left_to_next_head(char *left, char *head, int status)
{
	int		j;
	char	*tmp;

	j = 0;
	if (left)
	{
		while (left[j] != '\0')
		{
			if (left[j] == '$' && is_in_quote_dquote(left, j, status) != QUOTE)
				break ;
			j++;
		}
		tmp = ft_strjoin(head, xsubstr(left, 0, j, "expansion"));
		free(head);
		head = tmp;
	}
	return (head);
}

void	add_to_words(t_list **words, char *head, char *str)
{
	if (*words == NULL)
	{
		if (head != NULL)
			*words = xlstnew(head, "expansion");
		else
			*words = xlstnew(str, "expansion");
	}
	else
	{
		if (head != NULL)
			ft_lstadd_back(words, xlstnew(head, "expansion"));
		else
			ft_lstadd_back(words, xlstnew(str, "expansion"));
	}
}

char	*expand_redirect_target(t_cmd_block *cmd, t_list **words, int status)
{
	t_redirects	*redirects;

	redirects = cmd->redirects->content;
	printf("word: %s, words_last: %s\n", (char *)(*words)->content, (char *)ft_lstlast(*words)->content);
	if (status == 1)
	{
		print_error("expansion", EMESS_REDIRECT);
		exit(EXIT_FAILURE);
	}
	// free(redirects->target); //wordsの中身になりうる場合、free NG
	return ((char *)ft_lstlast(*words)->content);
	// free(*words);
}

// echo """'$TEST'""" ->echo "'le\"mon\"'" -> 'le"mon"'
// ($の前に”が奇数個存在する&&奇数個の最後の一つが'より前方に存在する) || ()
		// "$TEST"$TEST'$TEST'
int	find_param(t_environ *env, char *str, t_list **words)
{
	int		status;
	int		i;
	char	*head;
	char	*left;
	char	*param;
	char	*tmp; //除外する
	int		splitted;

	i = 0;
	status = NONE;
	head = NULL;
	splitted = 0;
	while (str[i] != '\0')
	{
		left = NULL;
		status = is_in_quote_dquote(str, i, status);
		if (status != QUOTE && str[i] == '$')
		{
			if (head == NULL && i != 0)
				head = xsubstr(str, 0, i, "expansion"); //aa
			param = xsubstr(str, i + 1, ft_strlen(str) - (i + 1) - get_left(str, i, &left), "expansion"); //PATH
			i += ft_strlen(param);
			// free(param); //以降必要ない場合は要フリー
			tmp = ft_strjoin(head, is_registered(env, &param, true));
			free(head);
			head = tmp;
			// printf("str: %s, i: %d, param: %s, param_len: %zu, head: %s, left: %s\n", str, i, param,ft_strlen(param), head, left);
			head = split_expanded_by_space(words, status, head, &splitted);
			head = left_to_next_head(left, head, status);
		}
		i++;
	}
	add_to_words(words, head, str);
	return (splitted);
}

// a$TE$TE'b'$TE | $USER$TE
t_list	*expansion(t_list *tokens, t_environ *env)
{
	t_cmd_block	*cmd;
	t_redirects	*redirects;
	t_list		*redirects_head;
	t_list		*words;
	t_list		*token_head;
	int			i;
	int			status;

	token_head = tokens;
	words = NULL;
	while (tokens != NULL)
	{
		cmd = tokens->content;
		if (cmd != NULL)
		{
			if (cmd->command != NULL)
			{
				status = find_param(env, cmd->command, &words);
				i = 1;
				while (cmd->args[i] != NULL)
				{
					status = find_param(env, cmd->args[i], &words);
					i++;
				}
				i = 0;
				if (words != NULL)
				{
					cmd->command = words->content; //呼び出された関数内で設定してもいいかも
					// 必要であればcmd->argsフリー
					cmd->args = malloc(sizeof(char *) * (ft_lstsize(words) + 1));
					while (words != NULL)
					{
						cmd->args[i] = words->content;
						words = words->next;
						i++;
					}
					cmd->args[i] = NULL;
					// ft_lstclear(&words_head, NULL);
				}
			}
			redirects_head = cmd->redirects;
			while (cmd->redirects != NULL)
			{
				redirects = cmd->redirects->content;
				status = find_param(env, redirects->target, &words);
				redirects->target = expand_redirect_target(cmd, &words, status);
				cmd->redirects = cmd->redirects->next;
			}
			cmd->redirects = redirects_head;
		}
		tokens = tokens->next;
	}
	return (token_head);
}
