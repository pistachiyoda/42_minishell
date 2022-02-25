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

TEST_GROUP(lexer_G)
{
	void setup() {
	}
	void teardown() {
	}
};

void	compare_words(char **expected, t_list *words)
{
	int		i;

	i = 0;
	words = words->next;
	while (words != NULL)
	{
		STRCMP_EQUAL(expected[i], (char *)(words->content));
		i++;
		words = words->next;
	}
}

char	**args3(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("cat");
	expected[1] = ft_strdup("test.txt");
	expected[2] = ft_strdup("'hoge.txt'");
	return (expected);
}

TEST(lexer_G, args3) {
	t_list	*words;
	char	**expected;

	words = lexer(ft_strdup("cat test.txt 'hoge.txt'"));
	expected = args3(3);
	compare_words(expected, words);
}

char	**redirect2_args6(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("echo");
	expected[1] = ft_strdup("a");
	expected[2] = ft_strdup("1");
	expected[3] = ft_strdup(">");
	expected[4] = ft_strdup("test.txt");
	expected[5] = ft_strdup("0");
	expected[6] = ft_strdup("<");
	expected[7] = ft_strdup("a.out");
	return (expected);
}

TEST(lexer_G, redirect2_args6) {
	t_list	*words;
	char	**expected;

	words = lexer(ft_strdup("echo a>test.txt<a.out"));
	expected = redirect2_args6(8);
	compare_words(expected, words);
}

char	**redirect1_pipe1_args7(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("echo");
	expected[1] = ft_strdup("a");
	expected[2] = ft_strdup("|");
	expected[3] = ft_strdup("echo");
	expected[4] = ft_strdup("b");
	expected[5] = ft_strdup("1");
	expected[6] = ft_strdup(">>");
	expected[7] = ft_strdup("c");
	return (expected);
}

TEST(lexer_G, redirect1_pipe1_args7) {
	t_list	*words;
	char	**expected;

	words = lexer(ft_strdup("echo a | echo b >> c"));
	expected = redirect1_pipe1_args7(8);
	compare_words(expected, words);
}

char	**heredoc(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("cat");
	expected[1] = ft_strdup("in");
	expected[2] = ft_strdup("0");
	expected[3] = ft_strdup("<<");
	expected[4] = ft_strdup("EOT");
	expected[5] = ft_strdup("wc");
	expected[6] = ft_strdup("-l");
	return (expected);
}

TEST(lexer_G, heredoc) {
	t_list	*words;
	char	**expected;

	words = lexer(ft_strdup("cat in<<EOT wc -l"));
	expected = heredoc(7);
	compare_words(expected, words);
}

char	**has_write_at_beginning(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("1");
	expected[1] = ft_strdup(">");
	expected[2] = ft_strdup("test.txt");
	expected[3] = ft_strdup("'hoge.txt'");
	return (expected);
}

TEST(lexer_G, has_write_at_beginning) {
	t_list	*words;
	char	**expected;

	words = lexer(ft_strdup(">test.txt 'hoge.txt'"));
	expected = has_write_at_beginning(4);
	compare_words(expected, words);
}

char	**has_append_at_beginning(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("1");
	expected[1] = ft_strdup(">>");
	expected[2] = ft_strdup("test.txt");
	expected[3] = ft_strdup("'hoge.txt'");
	return (expected);
}

TEST(lexer_G, has_append_at_beginning) {
	t_list	*words;
	char	**expected;

	words = lexer(ft_strdup("  >>  test.txt 'hoge.txt'"));
	expected = has_append_at_beginning(4);
	compare_words(expected, words);
}

char	**has_input_at_beginning(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("0");
	expected[1] = ft_strdup("<");
	expected[2] = ft_strdup("test.txt");
	expected[3] = ft_strdup("'hoge.txt'");
	return (expected);
}

TEST(lexer_G, has_input_at_beginning) {
	t_list	*words;
	char	**expected;

	words = lexer(ft_strdup("<  test.txt 'hoge.txt'"));
	expected = has_input_at_beginning(4);
	compare_words(expected, words);
}

char	**has_heredoc_at_beginning(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("0");
	expected[1] = ft_strdup("<<");
	expected[2] = ft_strdup("EOT");
	expected[3] = ft_strdup("'hoge.txt'");
	return (expected);
}

TEST(lexer_G, has_heredoc_at_beginning) {
	t_list	*words;
	char	**expected;

	words = lexer(ft_strdup("  <<EOT 'hoge.txt'"));
	expected = has_heredoc_at_beginning(4);
	compare_words(expected, words);
}

char	**has_space_at_beginning_end(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("cat");
	expected[1] = ft_strdup("test.txt");
	expected[2] = ft_strdup("'hoge.txt '");
	return (expected);
}

TEST(lexer_G, has_space_at_beginning_end) {
	t_list	*words;
	char	**expected;

	words = lexer(ft_strdup("  cat  test.txt   'hoge.txt '  "));
	expected = has_space_at_beginning_end(3);
	compare_words(expected, words);
}

char	**has_pipe_at_end(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("cat");
	expected[1] = ft_strdup("test.txt");
	expected[2] = ft_strdup("|");
	return (expected);
}

TEST(lexer_G, has_pipe_at_end) {
	t_list	*words;
	char	**expected;

	words = lexer(ft_strdup("cat test.txt|"));
	expected = has_pipe_at_end(3);
	compare_words(expected, words);
}

char	**has_fd_num(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("date");
	expected[1] = ft_strdup("1");
	expected[2] = ft_strdup("<");
	expected[3] = ft_strdup("in");
	return (expected);
}

TEST(lexer_G, has_fd_num) {
	t_list	*words;
	char	**expected;

	words = lexer(ft_strdup("date 1< in"));
	expected = has_fd_num(4);
	compare_words(expected, words);
}

char	**has_num_as_args(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("date");
	expected[1] = ft_strdup("1");
	expected[2] = ft_strdup("0");
	expected[3] = ft_strdup("<");
	expected[4] = ft_strdup("in");
	return (expected);
}

TEST(lexer_G, has_num_as_args) {
	t_list	*words;
	char	**expected;

	words = lexer(ft_strdup("date 1 < in"));
	expected = has_num_as_args(5);
	compare_words(expected, words);
}

char	**has_num_as_args_and_fd(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("date");
	expected[1] = ft_strdup("1");
	expected[2] = ft_strdup("0");
	expected[3] = ft_strdup("<");
	expected[4] = ft_strdup("in");
	return (expected);
}

TEST(lexer_G, has_num_as_args_and_fd) {
	t_list	*words;
	char	**expected;

	words = lexer(ft_strdup("date 1 0< in"));
	expected = has_num_as_args_and_fd(5);
	compare_words(expected, words);
}

//以下、異常系
TEST(lexer_G, redirect_at_end) {
	STRCMP_EQUAL(NULL, (char *)lexer(ft_strdup("cat test.txt>")));
}

TEST(lexer_G, redirect_in_a_row) {
	t_list	*words;

	words = lexer(ft_strdup("cat < ><>aa"));
	STRCMP_EQUAL(NULL, (char *)words);
}

TEST(lexer_G, redirect_in_a_row2) {
	t_list	*words;

	words = lexer(ft_strdup("cat <a><>aa"));
	STRCMP_EQUAL(NULL, (char *)words);
}

TEST(lexer_G, redirect_in_a_row3) {
	t_list	*words;

	words = lexer(ft_strdup("echo '<a>'<>aa"));
	STRCMP_EQUAL(NULL, (char *)words);
}

TEST(lexer_G, pipe_in_a_row) {
	t_list	*words;

	words = lexer(ft_strdup("echo '<a>'||aa"));
	STRCMP_EQUAL(NULL, (char *)words);
}

TEST(lexer_G, pipe_in_a_row2) {
	t_list	*words;

	words = lexer(ft_strdup("echo '<a>'| |aa"));
	STRCMP_EQUAL(NULL, (char *)words);
}

TEST(lexer_G, pipe_in_a_row3) {
	t_list	*words;

	words = lexer(ft_strdup("		| |aa"));
	STRCMP_EQUAL(NULL, (char *)words);
}

TEST(lexer_G, redirect_without_target) {
	t_list	*words;

	words = lexer(ft_strdup("cat input <| aa"));
	STRCMP_EQUAL(NULL, (char *)words);
}
