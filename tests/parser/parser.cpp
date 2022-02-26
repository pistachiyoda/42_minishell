#include <CppUTest/CommandLineTestRunner.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include "../test.h"

TEST_GROUP(parser_G)
{
	void setup() {
	}
	void teardown() {
	}
};

t_list	*get_tokens_from_parser(char *str)
{
	t_list		*tokens;
	t_cmd_block	*cmd;
	t_redirects	*redirects;

	tokens = parser(lexer(str));
	cmd = (t_cmd_block *)tokens->content;
	if (cmd->redirects)
		redirects = (t_redirects *)cmd->redirects->content;
	return (tokens);
}

void	compare_tokens(t_list *tokens, t_list *exp_tokens)
{
	int			i;
	t_cmd_block	*cmd;
	t_redirects	*redirects;
	t_cmd_block	*exp_cmd;
	t_redirects	*exp_redirects;

	while (exp_tokens != NULL)
	{
		i = 0;
		cmd = (t_cmd_block *)tokens->content;
		exp_cmd = (t_cmd_block *)exp_tokens->content;
		if (exp_cmd == NULL)
			STRCMP_EQUAL((char *)exp_cmd, (char *)cmd);
		else
		{
			STRCMP_EQUAL(exp_cmd->command, cmd->command);
			printf("cmd: %s\n", cmd->command);
			while (exp_cmd->args[i] != NULL)
			{
				printf("args[%d]: %s\n", i, cmd->args[i]);
				STRCMP_EQUAL(exp_cmd->args[i], cmd->args[i]);
				i++;
			}
		}
		while (exp_cmd != NULL && exp_cmd->redirects != NULL)
		{
			redirects = (t_redirects *)cmd->redirects->content;
			exp_redirects = (t_redirects *)exp_cmd->redirects->content;
			CHECK_EQUAL(exp_redirects->redirect, redirects->redirect);
			printf("type: %d\n", redirects->redirect);
			STRCMP_EQUAL(exp_redirects->target, redirects->target);
			printf("target: %s\n", redirects->target);
			CHECK_EQUAL(exp_redirects->fd, redirects->fd);
			printf("fd: %d\n", redirects->fd);
			cmd->redirects = cmd->redirects->next;
			exp_cmd->redirects = exp_cmd->redirects->next;
		}
		tokens = tokens->next;
		exp_tokens = exp_tokens->next;
	}
	printf("----------\n");
}

t_list	*args2(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("test.txt");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(parser_G, args2) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_parser(ft_strdup("cat test.txt"));
	exp_tokens = args2();
	compare_tokens(tokens, exp_tokens);
}

t_list	*redirect1_args4(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;
	t_redirects	*exp_redirects;

	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = WRITE;
	exp_redirects->target = ft_strdup("b");
	exp_redirects->fd = 1;
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redirects);
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("a");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(parser_G, redirect1_args4) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_parser(ft_strdup("echo a > b"));
	exp_tokens = redirect1_args4();
	compare_tokens(tokens, exp_tokens);
}

t_list	*pipe1_args5(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	// 1つ目のコマンド
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("a");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	// 2つ目のコマンド
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("wc");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("wc");
	exp_args[1] = ft_strdup("-l");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	ft_lstadd_back(&exp_tokens, ft_lstnew(exp_cmd));
	return (exp_tokens);
}

TEST(parser_G, pipe1_args5) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_parser(ft_strdup("echo a|wc -l"));
	exp_tokens = pipe1_args5();
	compare_tokens(tokens, exp_tokens);
}

t_list	*redirect3_pipe2_args14(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;
	t_redirects	*exp_redirects;

	// 1つ目のコマンド, 1つ目のリダイレクト
	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = WRITE;
	exp_redirects->target = ft_strdup("'b'");
	exp_redirects->fd = 1;
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redirects);
	// 2つ目のリダイレクト
	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = APPEND;
	exp_redirects->target = ft_strdup("c");
	exp_redirects->fd = 1;
	ft_lstadd_back(&exp_cmd->redirects, ft_lstnew(exp_redirects));
	// 3つ目のリダイレクト
	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = INPUT;
	exp_redirects->target = ft_strdup("d");
	exp_redirects->fd = 0;
	ft_lstadd_back(&exp_cmd->redirects, ft_lstnew(exp_redirects));
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("\"test\"");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	// 2つ目のコマンド
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("c");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	ft_lstadd_back(&exp_tokens, ft_lstnew(exp_cmd));
	// 3つ目のコマンド
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("wc");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("wc");
	exp_args[1] = ft_strdup("-l");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	ft_lstadd_back(&exp_tokens, ft_lstnew(exp_cmd));
	return (exp_tokens);
}

TEST(parser_G, redirect3_pipe2_args14) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_parser(ft_strdup("echo \"test\" > 'b'>>c<d| cat c| wc -l"));
	exp_tokens = redirect3_pipe2_args14();
	compare_tokens(tokens, exp_tokens);
}

t_list	*append_at_beginning(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;
	t_redirects	*exp_redirects;

	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = APPEND;
	exp_redirects->target = ft_strdup("cat");
	exp_redirects->fd = 1;
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redirects);
	exp_cmd->command = ft_strdup("a");
	exp_args = (char **)malloc(sizeof(char *) * 2);
	exp_args[0] = ft_strdup("a");
	exp_args[1] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(parser_G, append_at_beginning) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_parser(ft_strdup("1>> cat a"));
	exp_tokens = append_at_beginning();
	compare_tokens(tokens, exp_tokens);
}

t_list	*cmd_at_middle(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;
	t_redirects	*exp_redirects;

	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = WRITE;
	exp_redirects->target = ft_strdup("cat");
	exp_redirects->fd = 1;
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redirects);
	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = INPUT;
	exp_redirects->target = ft_strdup("b");
	exp_redirects->fd = 0;
	ft_lstadd_back(&exp_cmd->redirects, ft_lstnew(exp_redirects));
	exp_cmd->command = ft_strdup("a");
	exp_args = (char **)malloc(sizeof(char *) * 2);
	exp_args[0] = ft_strdup("a");
	exp_args[1] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(parser_G, cmd_at_middle) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_parser(ft_strdup("1> cat a 0<b"));
	exp_tokens = cmd_at_middle();
	compare_tokens(tokens, exp_tokens);
}

t_list	*cmd_at_end(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;
	t_redirects	*exp_redirects;

	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = INPUT;
	exp_redirects->target = ft_strdup("a");
	exp_redirects->fd = 0;
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redirects);
	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = APPEND;
	exp_redirects->target = ft_strdup("b");
	exp_redirects->fd = 1;
	ft_lstadd_back(&exp_cmd->redirects, ft_lstnew(exp_redirects));
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("b");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(parser_G, cmd_at_end) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_parser(ft_strdup("0<a 1>>b cat b  "));
	exp_tokens = cmd_at_end();
	compare_tokens(tokens, exp_tokens);
}

t_list	*fd_max(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;
	t_redirects	*exp_redirects;

	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = WRITE;
	exp_redirects->target = ft_strdup("a");
	exp_redirects->fd = 256;
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redirects);
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("123");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(parser_G, fd_max) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_parser(ft_strdup("echo 123 256>a"));
	exp_tokens = fd_max();
	compare_tokens(tokens, exp_tokens);
}

t_list	*fd_over_max(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;
	t_redirects	*exp_redirects;

	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = WRITE;
	exp_redirects->target = ft_strdup("a");
	exp_redirects->fd = 257;
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redirects);
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("123");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(parser_G, fd_over_max) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_parser(ft_strdup("echo 123 257>a"));
	exp_tokens = fd_over_max();
	compare_tokens(tokens, exp_tokens);
}

t_list	*fd_under_min(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;
	t_redirects	*exp_redirects;

	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = WRITE;
	exp_redirects->target = ft_strdup("a");
	exp_redirects->fd = 1;
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redirects);
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 4);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("123");
	exp_args[2] = ft_strdup("-1");
	exp_args[3] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(parser_G, fd_under_min) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_parser(ft_strdup("echo 123 -1>a"));
	exp_tokens = fd_under_min();
	compare_tokens(tokens, exp_tokens);
}

t_list	*heredoc(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;
	t_redirects	*exp_redirects;

	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = HEREDOC;
	exp_redirects->target = ft_strdup("EOT");
	exp_redirects->fd = 0;
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redirects);
	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = HEREDOC;
	exp_redirects->target = ft_strdup("EOT2");
	exp_redirects->fd = 0;
	ft_lstadd_back(&exp_cmd->redirects, ft_lstnew(exp_redirects));
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("-n");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(parser_G, heredoc) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_parser(ft_strdup("echo<< EOT<<EOT2  -n"));
	exp_tokens = heredoc();
	compare_tokens(tokens, exp_tokens);
}

t_list	*pipe_at_end(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("a");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	exp_cmd = NULL;
	ft_lstadd_back(&exp_tokens, ft_lstnew(exp_cmd));
	return (exp_tokens);
}

TEST(parser_G, pipe_at_end) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_parser(ft_strdup("echo a|"));
	exp_tokens = pipe_at_end();
	compare_tokens(tokens, exp_tokens);
}
