#include "minishell.h"

void	init_cmd_block(t_cmd_block *cmd)
{
	cmd = malloc(sizeof(t_cmd_block));
	malloc_check(cmd, "parser");
	cmd->command = NULL;
	cmd->args = NULL;
}

t_list	*is_redirect(t_list *words, t_list *redirects)
{
	t_redirects	*redir;
	t_list		*new_ele;
	size_t		len;

	while (words != NULL && (ft_strncmp(words->content, ">", 1) == 0
		|| ft_strncmp(words->content, ">>", 2) == 0
		|| ft_strncmp(words->content, "<", 1) == 0
		|| ft_strncmp(words->content, "<<", 2) == 0))
	{
		redir = malloc(sizeof(t_redirects));
		malloc_check(redir, "parser");
		//lenの長いほうで比較したほうがいいかも？想定ケースでは必要なさそうだけど
		len = ft_strlen(words->content);
		if (ft_strncmp(words->content, ">>", 2) == 0)
			redir->redirect = APPEND;
		else if (ft_strncmp(words->content, "<<", 2) == 0)
			redir->redirect = HEREDOC;
		else if (ft_strncmp(words->content, ">", 1) == 0)
			redir->redirect = WRITE;
		else if (ft_strncmp(words->content, "<", 1) == 0)
			redir->redirect = INPUT;
		words = words->next;
		//次がリダイレクトとかではなく正常なコマンドか確認必要(最初のエラー処理で実施)
		redir->target = words->content;
		if (redirects == NULL)
		{
			redirects = ft_lstnew(redir);
			malloc_check(redirects, "parser");
		}
		else
		{
			new_ele = ft_lstnew(redir);
			malloc_check(new_ele, "parser");
			ft_lstadd_back(&redirects, new_ele);
		}
		words = words->next;
	}
	return (redirects);
}

t_list	*parser(t_list *words)
{
	t_list		*tokens;
	t_list		*tmp_words;
	t_list		*new_ele;
	t_cmd_block	*cmd;
	int			count;
	int			i;

	words = words->next;
	tokens = NULL;
	if (ft_strncmp(words->content, "|", 1) == 0)
	{
		print_error("parser", EMESS_SYNTAX);
		return (NULL);
	}
	while (words != NULL)
	{
		i = 0;
		count = 0;
		cmd = malloc(sizeof(t_cmd_block));
		malloc_check(cmd, "parser");
		cmd->redirects = is_redirect(words, cmd->redirects);
		printf("words1: %s\n", (char *)words->content);
		tmp_words = words;
		cmd->command = words->content;
		while (tmp_words != NULL && ft_strncmp(tmp_words->content, "|", 1) != 0
			&& ft_strncmp(tmp_words->content, ">", 1) != 0
			&& ft_strncmp(tmp_words->content, ">>", 2) != 0
			&& ft_strncmp(tmp_words->content, "<", 1) != 0
			&& ft_strncmp(tmp_words->content, "<<", 2) != 0) //このリダイレクトの条件直前で弾いてるから不要かも
		{
			count++;
			tmp_words = tmp_words->next;
		}
		cmd->args = malloc(sizeof(char *) * (count + 1));
		malloc_check(cmd->args, "parser");
		printf("count: %d\n", count);
		while (i < count)
		{
			cmd->args[i] = words->content;
			words = words->next;
			i++;
		}
		cmd->args[i] = NULL;
		cmd->redirects = is_redirect(words, cmd->redirects);
		printf("pass here\n");
		// t_redirects *test;
		// test = cmd->redirects->content;
		// printf("type: %d, target: %s\n", test->redirect, test->target);
		if (tokens == NULL)
		{
			tokens = ft_lstnew(cmd);
			malloc_check(tokens, "parser");
		}
		else
		{
			new_ele = ft_lstnew(cmd);
			malloc_check(new_ele, "parser");
			ft_lstadd_back(&tokens, new_ele);
		}
		if (words != NULL)
			printf("words3: %s\n", (char *)words->content);
		if (words != NULL && ft_strncmp(words->content, "|", 1) == 0)
		{
			printf("PIPE found\n");
			words = words->next;
		}
		else
			break ;
	}
	return (tokens);
}

t_list	*set_args(char *str)
{
	t_list	*words;
	t_list	*tokens;
	t_cmd_block	*cmd;
	t_redirects	*redirects;
	int	i;

	//redirect, pipe終わりだった場合、次の処理に進まないようにする
	words = lexer(str);
	if (words == NULL)
		return (NULL);
	t_list *lexer;
	lexer = words->next;
	while (lexer != NULL)
	{
		printf("lexer: %s\n", (char *)lexer->content);
		lexer = lexer->next;
	}
	tokens = parser(words);
	printf("PASS\n");
	while (tokens != NULL)
	{
		i = 0;
		cmd = tokens->content;
		printf("command: %s\n", cmd->command);
		while (cmd->args[i] != NULL)
		{
			printf("args[%d]: %s\n", i, cmd->args[i]);
			i++;
		}
		printf("====\n");
		while (cmd->redirects != NULL)
		{
			redirects = cmd->redirects->content;
			printf("type: %d, target: %s\n", redirects->redirect, redirects->target);
			cmd->redirects = cmd->redirects->next;
		}
		if (tokens->next == NULL)
			break ;
		tokens = tokens->next;
	}
	// 変数展開の関数呼ぶ
	return(tokens);
}
