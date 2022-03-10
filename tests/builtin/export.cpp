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

TEST_GROUP(export_G)
{
	void setup() {
	}
	void teardown() {
	}
};

t_list	*export_empty_val(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("[a]");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(export_G, export_empty_val) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export SPACE=\"\""), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("echo [$SPACE'a']"), env);
	exp_tokens = export_empty_val();
	compare_tokens(tokens, exp_tokens);
}

t_list	*export_empty_val2(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("[a]");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(export_G, export_empty_val2) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export SPACE=''"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("echo [$SPACE'a']"), env);
	exp_tokens = export_empty_val2();
	compare_tokens(tokens, exp_tokens);
}

t_list	*export_empty_val3(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("[a]");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(export_G, export_empty_val3) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export SPACE="), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("echo [$SPACE'a']"), env);
	exp_tokens = export_empty_val3();
	compare_tokens(tokens, exp_tokens);
}

t_list	*export_NULL_val(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("[a]");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(export_G, export_NULL_val) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export SPACE"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("echo [$SPACE'a']"), env);
	exp_tokens = export_NULL_val();
	compare_tokens(tokens, exp_tokens);
}

t_list	*export_space_val(void)
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
	return (exp_tokens);
}

TEST(export_G, export_space_val) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export SPACE=\"   \""), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("echo $SPACE'a'"), env);
	exp_tokens = export_space_val();
	compare_tokens(tokens, exp_tokens);
}

t_list	*export_space_val2(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 4);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("a");
	exp_args[2] = ft_strdup("b");
	exp_args[3] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(export_G, export_space_val2) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export SPACE=\"   \""), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("echo a$SPACE'b'"), env);
	exp_tokens = export_space_val2();
	compare_tokens(tokens, exp_tokens);
}

t_list	*export_space_val3(void)
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
	return (exp_tokens);
}

TEST(export_G, export_space_val3) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export SPACE=\"   \""), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("echo a$SPACE"), env);
	exp_tokens = export_space_val3();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_split_env_with_spaces(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 4);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("test");
	exp_args[2] = ft_strdup("file");
	exp_args[3] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(export_G, has_split_env_with_spaces) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export SPLIT_ENV=\"  test  file  \""), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("  echo  $SPLIT_ENV  "), env);
	exp_tokens = has_split_env_with_spaces();
	compare_tokens(tokens, exp_tokens);
}

t_list	*has_split_env_with_spaces2(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 4);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("test");
	exp_args[2] = ft_strdup("filesdaqw  ");
	exp_args[3] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(export_G, has_split_env_with_spaces2) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export SPLIT_ENV=\"  test  file\""), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("  echo  $SPLIT_ENV'sda''qw''  '  "), env);
	exp_tokens = has_split_env_with_spaces2();
	compare_tokens(tokens, exp_tokens);
}

t_list	*export_equal_signs_val(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("=a=b=c==d=e");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(export_G, export_equal_signs_val) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export EQUAL==a=b=c==d="), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("echo $EQUAL'e'"), env);
	exp_tokens = export_equal_signs_val();
	compare_tokens(tokens, exp_tokens);
}

t_list	*export_same_keys(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("new");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(export_G, export_same_keys) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export TEST=old TEST=new"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("echo $TEST"), env);
	exp_tokens = export_same_keys();
	compare_tokens(tokens, exp_tokens);
}

t_list	*check_exit_status(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("0");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(export_G, check_exit_status) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export VALID_ARG"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("echo $?"), env);
	exp_tokens = check_exit_status();
	compare_tokens(tokens, exp_tokens);
}

t_list	*check_exit_status2(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("0");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(export_G, check_exit_status2) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export VALID_ARG=valid"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("echo $?"), env);
	exp_tokens = check_exit_status2();
	compare_tokens(tokens, exp_tokens);
}

t_list	*check_exit_status3(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("0");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(export_G, check_exit_status3) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export UESR"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("echo $?"), env);
	exp_tokens = check_exit_status3();
	compare_tokens(tokens, exp_tokens);
}

t_list	*check_exit_status_error(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("1");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(export_G, check_exit_status_error) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export TEST 0invalid TEST2"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("echo $?"), env);
	exp_tokens = check_exit_status_error();
	compare_tokens(tokens, exp_tokens);
}

t_list	*check_exit_status_error2(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("1");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(export_G, check_exit_status_error2) {
	t_list		*tokens;
	t_list		*exp_tokens;
	t_environ	*env;

	env = create_environ(g_envp);
	tokens = get_tokens_from_expansion(ft_strdup("export TEST _invalid* TEST2"), env);
	ft_export((t_cmd_block *)tokens->content, env);
	tokens = get_tokens_from_expansion(ft_strdup("echo $?"), env);
	exp_tokens = check_exit_status_error2();
	compare_tokens(tokens, exp_tokens);
}
