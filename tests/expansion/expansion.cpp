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

TEST_GROUP(expansion_G)
{
	void setup() {
	}
	void teardown() {
	}
};

t_list	*normal_(void) //
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

// TEST(expansion_G, normal_) {
// 	t_list		*tokens;
// 	t_list		*exp_tokens;

// 	tokens = expansion(parser(lexer(ft_strdup("ca't' 'test.txt'"));
// 	exp_tokens = normal();
// 	compare_tokens(tokens, exp_tokens);
// }

t_list	*normal(void)
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

TEST(expansion_G, normal) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = expansion(parser(lexer(ft_strdup("cat test.txt"))), env);
	exp_tokens = normal();
	compare_tokens(tokens, exp_tokens);
}

t_list	*normal2(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	t_redirects	*exp_redir;
	char		**exp_args;

	exp_redir = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redir->fd = 1;
	exp_redir->redirect = WRITE;
	exp_redir->target = ft_strdup("a.out");
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redir);
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("test.txt");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, normal2) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = expansion(parser(lexer(ft_strdup("  cat test.txt >a.out  "))), env);
	exp_tokens = normal();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_env(void) //export FF="testfile"ってされてもremove_quoteでクオート消されるから問題なし
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	t_redirects	*exp_redir;
	char		**exp_args;

	exp_redir = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redir->fd = 1;
	exp_redir->redirect = WRITE;
	exp_redir->target = ft_strdup("testfile");
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redir);
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("test.txt");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, has_env) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = expansion(parser(lexer(ft_strdup("export FF=testfile"))), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = expansion(parser(lexer(ft_strdup("  cat test.txt >$FF  "))), env);
	exp_tokens = has_env();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_split_env(void) //
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 5);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("test.txt");
	exp_args[2] = ft_strdup("\"test");
	exp_args[3] = ft_strdup("file\"");
	exp_args[4] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, has_split_env) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = expansion(parser(lexer(ft_strdup("export SPLIT_ENV=\"test file\""))), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = expansion(parser(lexer(ft_strdup("  cat test.txt $SPLIT_ENV  "))), env);
	exp_tokens = has_split_env();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_dquoted_env(void) //
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	t_redirects	*exp_redir;
	char		**exp_args;

	exp_redir = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redir->fd = 1;
	exp_redir->redirect = WRITE;
	exp_redir->target = ft_strdup("\"testfile\"");
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redir);
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("test.txt");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, has_dquoted_env) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = expansion(parser(lexer(ft_strdup("export FF=testfile"))), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = expansion(parser(lexer(ft_strdup("  cat test.txt >\"$FF\"  "))), env);
	exp_tokens = has_dquoted_env();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_multiple_env(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	t_redirects	*exp_redir;
	char		**exp_args;

	exp_redir = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redir->fd = 1;
	exp_redir->redirect = WRITE;
	exp_redir->target = ft_strdup("\"testfile\"");
	// exp_redir->target = ft_strdup("testfile");
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redir);
	exp_cmd->command = ft_strdup("aa\"cat");
	exp_args = (char **)malloc(sizeof(char *) * 4);
	exp_args[0] = ft_strdup("aa\"cat");
	exp_args[1] = ft_strdup("a\"\"cat");
	exp_args[2] = ft_strdup("b\"'b'\"cat");
	exp_args[3] = ft_strdup("b\"'cc'");
	// exp_args[0] = ft_strdup("aacat");
	// exp_args[1] = ft_strdup("acat");
	// exp_args[2] = ft_strdup("b'b'cat");
	// exp_args[3] = ft_strdup("b'cc'");
	exp_args[4] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, has_multiple_env) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = expansion(parser(lexer(ft_strdup("export CAT_A=\"cat a\""))), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = expansion(parser(lexer(ft_strdup("export CAT_B=\"cat b\""))), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = expansion(parser(lexer(ft_strdup("export FF=\"testfile\""))), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = expansion(parser(lexer(ft_strdup("aa$CAT_A$CAT_B'b'$CAT_B'cc' > $FF"))), env);
	exp_tokens = has_multiple_env();
	compare_tokens(tokens, exp_tokens);
}
