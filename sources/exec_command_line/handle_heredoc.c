#include "minishell.h"

void	handle_unused_heredoc(char *str)
{
	free(str);
	exit(0);
}

void	flush_heredoc(char *str, int doc_pipe_fds[2])
{
	char	*tmp;

	tmp = str;
	close_wrapper(doc_pipe_fds[0]);
	if (write(doc_pipe_fds[1], str, ft_strlen(str)) == -1)
	{
		perror("write()");
		exit(1);
	}
	close_wrapper(doc_pipe_fds[1]);
	free(tmp);
	exit(0);
}

// testが渡ってくる
// $~が展開できたら展開したやつを返す 
// 展開できなかったら$~を返す　→ $test
char	*expand_env_variables(t_environ *env, char *key)
{
	char *ret;

	ret = get_env_val(key, t_environ_to_vector(env));
	if (ret == NULL)
		return (ft_strdup(""));
	return (ret);
}

// 文字列と$の位置のindexと受け取る
// $の次の文字から区切り文字までを取得して返す
// 区切り文字どこまで対応すべきか調査
// $test hogehoge -> test
char	*get_key(char *buf, int *i)
{
	int		cnt;
	char	*key;
	int		j;

	printf("+++ in get_key +++\n");
	printf("i = %d\n", *i);
	cnt = 0;
	// $直後から$, スペース、改行までの文字数をカウント
	(*i) ++;
	while (buf[*i] != '$' && buf[*i] != ' ' && buf[*i] != '\n' && buf[*i] != '\0')
	{
		printf("buf[%d] = %c\n", *i, buf[*i]);
		cnt ++;
		(*i) ++;
	}
	printf("+++ end while +++\n");
	// $直後から$, スペース、改行までの文字を*keyに入れる
	key = (char *)malloc(sizeof(char *) * (cnt + 1)); // NULL処理、　free
	(*i) -= cnt;
	printf("i = %d\n", *i);
	j = 0;
	while (j < cnt)
	{
		key[j] = buf[*i];
		j ++;
		(*i) ++;
	}
	printf("i = %d\n", *i);
	key[j] = '\0';
	(*i) --;
	return (key);
}

// $$PATHみたいなケース
// 区切り文字スペース以外にももっと考慮したほうがいいかも？
char	*expand_env_variables_in_buf(t_environ *env, char *buf)
{
	int		i;
	int		j;
	int		cnt;
	char	*tmp_str;
	char	*value;
	char	*joined_str;
	
	i = 0;
	joined_str = ft_strdup(""); // NULL処理
	// todo:＄が登場しなかった場合の処理
	cnt = 0;
	while (buf[i])
	{
		printf("buf[%d] = %c\n", i, buf[i]);
		if (buf[i] == '$')
		{
			// それまでの文字列を何かに入れる　1文字目〜$まで、　または環境変数後〜
			tmp_str = (char *)malloc(sizeof(char *) * cnt); //２週目いこうの処理 // free
			i -= cnt;
			j = 0;
			while (j < cnt)
			{
				tmp_str[j] = buf[i];
				i ++;
				j ++;
			}
			// valueの取得
			value = expand_env_variables(env, get_key(buf, &i));
			// valueと繋げる
			joined_str = ft_strjoin_wrapper(joined_str, value);
		printf("i = %d\n", i);
			printf("joined_str = %s\n", joined_str);
			cnt = 0;
		}
		else
		{
			tmp_str = (char *)malloc(sizeof(char *) * 2);
			tmp_str[0] = buf[i];
			tmp_str[1] = '\0';
			joined_str = ft_strjoin_wrapper(joined_str, tmp_str);
			printf("joined_str = %s\n", joined_str);
		}

		i++;
		cnt ++;
	}
	printf("ret = %s\n", joined_str);
	return (joined_str);
}

void	handle_each_input(t_environ *env, char *limiter, bool is_last, int	doc_pipe_fds[2])
{
	char	*str;
	char	*buf;
	char	*expanded_buf;
	char	*tmp;
	(void)env;

	str = ft_strdup("");
	if (str == NULL)
		exit(1);
	while (1)
	{
		buf = readline("> ");
		if ((ft_strlen(buf) == ft_strlen(limiter))
			&& ft_strncmp(buf, limiter, ft_strlen(limiter)) == 0)
			break ;
		tmp = str;
		expanded_buf = expand_env_variables_in_buf(env, buf);
		str = ft_strjoin2(str, expanded_buf);
		free(buf);
		free(tmp);
	}
	if (!is_last)
		handle_unused_heredoc(str);
	flush_heredoc(str, doc_pipe_fds);
}

void	handle_heredoc(t_environ *env, char *limiter, bool is_last, int doc_pipe_fds[2])
{
	int		pid;

	pipe_wrapper(doc_pipe_fds);
	pid = fork_wrapper();
	if (pid == 0)
		handle_each_input(env, limiter, is_last, doc_pipe_fds);
	close_wrapper(doc_pipe_fds[1]);
	waitpid_wrapper(pid, NULL, 0);
}

int	handle_heredoc_input(t_environ *env, t_list *cmd_list)
{
	t_cmd_block	*cmd_block;
	t_list		*redirect_node;
	t_redirects	*redirect;
	int			doc_pipe_fds[2];

	while (cmd_list)
	{
		cmd_block = (t_cmd_block *)cmd_list->content;
		redirect_node = cmd_block->redirects;
		while (redirect_node)
		{
			redirect = redirect_node->content;
			if (redirect->redirect == HEREDOC)
			{
				handle_heredoc(env, redirect->target,
					is_last_fd_input_redirect(
						redirect, cmd_block->redirects),
					doc_pipe_fds);
				redirect->doc_fd = doc_pipe_fds[0];
			}
			redirect_node = redirect_node->next;
		}
		cmd_list = cmd_list->next;
	}
	return (0);
}
