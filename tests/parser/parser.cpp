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

	while (tokens != NULL)
	{
		i = 0;
		cmd = (t_cmd_block *)tokens->content;
		exp_cmd = (t_cmd_block *)exp_tokens->content;
		STRCMP_EQUAL(exp_cmd->command, cmd->command);
		while (cmd->args[i] != NULL)
		{
			STRCMP_EQUAL(exp_cmd->args[i], cmd->args[i]);
			i++;
		}
		while (cmd->redirects != NULL)
		{
			redirects = (t_redirects *)cmd->redirects->content;
			exp_redirects = (t_redirects *)exp_cmd->redirects->content;
			CHECK_EQUAL(exp_redirects->redirect, redirects->redirect);
			STRCMP_EQUAL(exp_redirects->target, redirects->target);
			cmd->redirects = cmd->redirects->next;
			exp_cmd->redirects = exp_cmd->redirects->next;
		}
		tokens = tokens->next;
		exp_tokens = exp_tokens->next;
	}
}

t_list	*args2(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;
	t_redirects	*exp_redirects;

	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects = NULL;
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redirects);
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 2);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("test.txt");
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

//cat test.txt
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
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redirects);
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 4);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("a");
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

// echo a > b
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
	t_redirects	*exp_redirects;

	// 1つ目のコマンド
	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = WRITE;
	exp_redirects->target = ft_strdup("b");
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redirects);
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 2);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("a");
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	// 2つ目のコマンド
	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects = NULL;
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redirects);
	exp_cmd->command = ft_strdup("wc");
	exp_args = (char **)malloc(sizeof(char *) * 2);
	exp_args[0] = ft_strdup("wc");
	exp_args[1] = ft_strdup("-l");
	exp_cmd->args = exp_args;
	ft_lstadd_back(&exp_tokens, ft_lstnew(exp_cmd));
	return (exp_tokens);
}

// echo a|wc -l
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
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redirects);
	// 2つ目のリダイレクト
	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = APPEND;
	exp_redirects->target = ft_strdup("c");
	ft_lstadd_back(&exp_cmd->redirects, ft_lstnew(exp_redirects));
	// 2つ目のリダイレクト
	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects->redirect = INPUT;
	exp_redirects->target = ft_strdup("d");
	ft_lstadd_back(&exp_cmd->redirects, ft_lstnew(exp_redirects));
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 2);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("\"test\"");
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	// 2つ目のコマンド
	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects = NULL;
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redirects);
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 2);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("c");
	exp_cmd->args = exp_args;
	ft_lstadd_back(&exp_tokens, ft_lstnew(exp_cmd));
	// 3つ目のコマンド
	exp_redirects = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redirects = NULL;
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redirects);
	exp_cmd->command = ft_strdup("wc");
	exp_args = (char **)malloc(sizeof(char *) * 2);
	exp_args[0] = ft_strdup("wc");
	exp_args[1] = ft_strdup("-l");
	exp_cmd->args = exp_args;
	ft_lstadd_back(&exp_tokens, ft_lstnew(exp_cmd));
	return (exp_tokens);
}

// echo "test" > 'b'>>c<d| cat c | wc -l
TEST(parser_G, redirect3_pipe2_args14) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_parser(ft_strdup("echo \"test\" > 'b'>>c<d| cat c"));
	exp_tokens = redirect3_pipe2_args14();
	compare_tokens(tokens, exp_tokens);
}
