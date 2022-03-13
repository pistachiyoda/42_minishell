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

t_list	*get_tokens_from_expansion(char *str, t_environ *env)
{
	t_list	*tokens;
	t_list	*words;

	words = NULL;
	tokens = NULL;
	lexer(str, &words);
	parser(words, &tokens, str);
	expansion(&tokens, env);
	return (tokens);
}

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

	tokens = get_tokens_from_expansion(ft_strdup("cat test.txt"),
		create_environ(g_envp));
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

	tokens = get_tokens_from_expansion(ft_strdup("  cat test.txt >a.out  "),
		create_environ(g_envp));
	exp_tokens = normal2();
	compare_tokens(tokens, exp_tokens);
}

t_list	*normal3(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	t_redirects	*exp_redir;
	t_redirects	*exp_redir2;
	char		**exp_args;

	exp_redir = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redir->fd = 0;
	exp_redir->redirect = INPUT;
	exp_redir->target = ft_strdup("in");
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redir);
	exp_redir2 = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redir2->fd = 0;
	exp_redir2->redirect = INPUT;
	exp_redir2->target = ft_strdup("in2");
	ft_lstadd_back(&exp_cmd->redirects, ft_lstnew(exp_redir2));
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 2);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, normal3) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_expansion(ft_strdup("cat < in < in2"),
		create_environ(g_envp));
	exp_tokens = normal3();
	compare_tokens(tokens, exp_tokens);
}

t_list	*normal4(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	t_redirects	*exp_redir;
	t_redirects	*exp_redir2;
	t_redirects	*exp_redir3;
	char		**exp_args;

	exp_redir = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redir->fd = 0;
	exp_redir->redirect = INPUT;
	exp_redir->target = ft_strdup("in");
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redir);
	exp_redir2 = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redir2->fd = 0;
	exp_redir2->redirect = INPUT;
	exp_redir2->target = ft_strdup("in2");
	ft_lstadd_back(&exp_cmd->redirects, ft_lstnew(exp_redir2));
	exp_redir3 = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redir3->fd = 0;
	exp_redir3->redirect = INPUT;
	exp_redir3->target = ft_strdup("in3");
	ft_lstadd_back(&exp_cmd->redirects, ft_lstnew(exp_redir3));
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 2);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, normal4) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_expansion(ft_strdup("cat < in < in2 < in3"),
		create_environ(g_envp));
	exp_tokens = normal4();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_pipe_redirects_in_dquote(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("cat test.txt | >test.txt ");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, has_pipe_redirects_in_dquote) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_expansion(ft_strdup("echo \"cat test.txt | >test.txt \""),
		create_environ(g_envp));
	exp_tokens = has_pipe_redirects_in_dquote();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_env(void)
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
	tokens = get_tokens_from_expansion(ft_strdup("export FF=testfile"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("  cat test.txt >$FF  "), env);
	exp_tokens = has_env();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_split_env(void)
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
	exp_args[2] = ft_strdup("test");
	exp_args[3] = ft_strdup("file");
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
	tokens = get_tokens_from_expansion(ft_strdup("export SPLIT_ENV=\"test file\""), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("  cat test.txt $SPLIT_ENV  "), env);
	exp_tokens = has_split_env();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_dquoted_env(void)
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

TEST(expansion_G, has_dquoted_env) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export FF=testfile"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("  cat test.txt >\"$FF\"  "), env);
	exp_tokens = has_dquoted_env();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_dquoted_env2(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("'test'");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, has_dquoted_env2) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export TEST=test"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("cat \"'$TEST'\""), env);
	exp_tokens = has_dquoted_env2();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_dquoted_env3(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("'  test  '");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, has_dquoted_env3) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export TEST=test"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("cat \"'  $TEST  '\""), env);
	exp_tokens = has_dquoted_env3();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_dquoted_env4(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup(" '  test  ' ");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, has_dquoted_env4) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export TEST=test"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("cat \" '  $TEST  ' \""), env);
	exp_tokens = has_dquoted_env4();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_dquoted_env5(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("cat  ' test' ");
	exp_args = (char **)malloc(sizeof(char *) * 2);
	exp_args[0] = ft_strdup("cat  ' test' ");
	exp_args[1] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, has_dquoted_env5) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export TEST=test"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("\"cat  ' $TEST' \""), env);
	exp_tokens = has_dquoted_env5();
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
	exp_redir->target = ft_strdup("testfile");
	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = ft_lstnew(exp_redir);
	exp_cmd->command = ft_strdup("aacat");
	exp_args = (char **)malloc(sizeof(char *) * 5);
	exp_args[0] = ft_strdup("aacat");
	exp_args[1] = ft_strdup("acat");
	exp_args[2] = ft_strdup("bbcat");
	exp_args[3] = ft_strdup("bcc");
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
	tokens = get_tokens_from_expansion(ft_strdup("export CAT_A=\"cat a\""), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("export CAT_B=\"cat b\""), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("export FF=\"testfile\""), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("aa$CAT_A$CAT_B'b'$CAT_B'cc' > $FF"), env);
	exp_tokens = has_multiple_env();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_quote(void)
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

TEST(expansion_G, has_quote) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("ca't' 'test.txt'"), env);
	exp_tokens = has_quote();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_quote2(void)
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

TEST(expansion_G, has_quote2) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("ca't' 'test.t'x't'''"), env);
	exp_tokens = has_quote2();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_quote3(void)
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

TEST(expansion_G, has_quote3) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("ca't' 'test.t''x't"), env);
	exp_tokens = has_quote3();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_quote4(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 4);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("test.txt");
	exp_args[2] = ft_strdup("  ");
	exp_args[3] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, has_quote4) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("ca't' 'test.t''x't \"  \""), env);
	exp_tokens = has_quote4();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_quote5(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 4);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("test.txt");
	exp_args[2] = ft_strdup("   ");
	exp_args[3] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, has_quote5) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export SPACE=\"   \""), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("ca't' 'test.t''x't \"$SPACE\""), env);
	exp_tokens = has_quote5();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_quote6(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 4);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("test.txt");
	exp_args[2] = ft_strdup("$SPACE");
	exp_args[3] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, has_quote6) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export SPACE=\"   \""), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("ca't' 'test.t''x't '$SPACE'"), env);
	exp_tokens = has_quote6();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_quote7(void)
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

TEST(expansion_G, has_quote7) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export SPACE=\"   \""), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("ca't' 'test.t''x't $SPACE"), env);
	exp_tokens = has_quote7();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_quote8(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 6);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("test.txt");
	exp_args[2] = ft_strdup("space");
	exp_args[3] = ft_strdup("space");
	exp_args[4] = ft_strdup("space");
	exp_args[5] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, has_quote8) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export SPACE=\" space    space   space  \""), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("ca't' 'test.t''x't $SPACE"), env);
	exp_tokens = has_quote8();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_quote9(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 6);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("test.txt");
	exp_args[2] = ft_strdup("space");
	exp_args[3] = ft_strdup("space");
	exp_args[4] = ft_strdup("space");
	exp_args[5] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, has_quote9) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export SPACE=\" space    space   space  \""), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("ca't' 'test.t''x't $SPACE   "), env);
	exp_tokens = has_quote9();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_signle_quoted_env(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("$TEST");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, has_signle_quoted_env) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export TEST=test"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("cat '$TEST'"), env);
	exp_tokens = has_signle_quoted_env();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_signle_quoted_env2(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("  $TEST  ");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, has_signle_quoted_env2) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export TEST=test"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("cat '  $TEST  '"), env);
	exp_tokens = has_signle_quoted_env2();
	compare_tokens(tokens, exp_tokens);
}

t_list	*export_env(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 4);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("-n");
	exp_args[2] = ft_strdup("abc");
	exp_args[3] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, export_env) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export CHO=\"cho -n\""), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("e$CHO abc"), env);
	exp_tokens = export_env();
	compare_tokens(tokens, exp_tokens);
}

t_list	*heredoc_target(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	t_redirects	*exp_redir;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_redir = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redir->fd = 0;
	exp_redir->redirect = HEREDOC;
	exp_redir->target = ft_strdup("$ABC");
	exp_cmd->redirects = ft_lstnew(exp_redir);
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("abc");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;

	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, heredoc_target) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export ABC=abc"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("cat $ABC << $ABC"), env);
	exp_tokens = heredoc_target();
	compare_tokens(tokens, exp_tokens);
}

t_list	*heredoc_target2(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	t_redirects	*exp_redir;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_redir = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redir->fd = 0;
	exp_redir->redirect = QUOTED_HEREDOC;
	exp_redir->target = ft_strdup("$ABC");
	exp_cmd->redirects = ft_lstnew(exp_redir);
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("abc");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;

	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, heredoc_target2) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export ABC=abc"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("cat \"$ABC\" << \"$ABC\""), env);
	exp_tokens = heredoc_target2();
	compare_tokens(tokens, exp_tokens);
}

t_list	*heredoc_target3(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	t_redirects	*exp_redir;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_redir = (t_redirects *)malloc(sizeof(t_redirects));
	exp_redir->fd = 0;
	exp_redir->redirect = QUOTED_HEREDOC;
	exp_redir->target = ft_strdup("$ABC");
	exp_cmd->redirects = ft_lstnew(exp_redir);
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("$ABC");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;

	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(expansion_G, heredoc_target3) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export ABC=abc"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("cat '$ABC' << '$ABC'"), env);
	exp_tokens = heredoc_target3();
	compare_tokens(tokens, exp_tokens);
}
