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

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("cat test.txt 'hoge.txt'"), &words));
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

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("echo a>test.txt<a.out"), &words));
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

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("echo a | echo b >> c"), &words));
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

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("cat in<<EOT wc -l"), &words));
	expected = heredoc(7);
	compare_words(expected, words);
}

char	**heredoc2(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("echo");
	expected[1] = ft_strdup("0");
	expected[2] = ft_strdup("<<");
	expected[3] = ft_strdup("EOT");
	expected[4] = ft_strdup("0");
	expected[5] = ft_strdup("<<");
	expected[6] = ft_strdup("EOT2");
	expected[7] = ft_strdup("-n");
	expected[8] = NULL;
	return (expected);
}

TEST(lexer_G, heredoc2) {
	t_list	*words;
	char	**expected;

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("echo<< EOT<<EOT2 -n"), &words));
	expected = heredoc2(9);
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

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup(">test.txt 'hoge.txt'"), &words));
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

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("  >>  test.txt 'hoge.txt'"), &words));
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

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("<  test.txt 'hoge.txt'"), &words));
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

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("  <<EOT 'hoge.txt'"), &words));
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

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("  cat  test.txt   'hoge.txt '  "), &words));
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

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("cat test.txt|"), &words));
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

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("date 1< in"), &words));
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

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("date 1 < in"), &words));
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

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("date 1 0< in"), &words));
	expected = has_num_as_args_and_fd(5);
	compare_words(expected, words);
}

char	**fd_under_min(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("date");
	expected[1] = ft_strdup("-1");
	expected[2] = ft_strdup("0");
	expected[3] = ft_strdup("<");
	expected[4] = ft_strdup("in");
	return (expected);
}

TEST(lexer_G, fd_under_min) {
	t_list	*words;
	char	**expected;

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("date -1< in"), &words));
	expected = fd_under_min(5);
	compare_words(expected, words);
}

char	**fd_over_max(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("date");
	expected[1] = ft_strdup("257");
	expected[2] = ft_strdup(">");
	expected[3] = ft_strdup("in");
	return (expected);
}

TEST(lexer_G, fd_over_max) {
	t_list	*words;
	char	**expected;

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("date 257> in"), &words));
	expected = fd_over_max(5);
	compare_words(expected, words);
}

char	**fd_max(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("date");
	expected[1] = ft_strdup("256");
	expected[2] = ft_strdup(">");
	expected[3] = ft_strdup("in");
	return (expected);
}

TEST(lexer_G, fd_max) {
	t_list	*words;
	char	**expected;

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("date 256> in"), &words));
	expected = fd_max(4);
	compare_words(expected, words);
}

char	**has_digit_target(int num)
{
	char	**expected;

	expected = (char **)malloc(sizeof(char *) * num);
	expected[0] = ft_strdup("cat");
	expected[1] = ft_strdup("input");
	expected[2] = ft_strdup("0");
	expected[3] = ft_strdup("<");
	expected[4] = ft_strdup("0");
	expected[5] = ft_strdup("1");
	expected[6] = ft_strdup(">");
	expected[7] = ft_strdup("aa");
	return (expected);
}

TEST(lexer_G, has_digit_target) {
	t_list	*words;
	char	**expected;

	words = NULL;
	CHECK_EQUAL(true, lexer(ft_strdup("cat input <0 > aa"), &words));
	expected = has_digit_target(8);
	compare_words(expected, words);
}

//以下、異常系: false時は、wordsはfree後でNULL止めもしてないからwordsのテスト省略
TEST(lexer_G, redirect_at_end) {
	t_list	*words;

	words = NULL;
	CHECK_EQUAL(false, lexer(ft_strdup("cat test.txt>"), &words));
}

TEST(lexer_G, unclosed_quote) {
	t_list	*words;

	words = NULL;
	CHECK_EQUAL(false, lexer(ft_strdup("cat test.txt'"), &words));
}

TEST(lexer_G, unclosed_quote2) {
	t_list	*words;

	words = NULL;
	CHECK_EQUAL(false, lexer(ft_strdup("cat test.txt\""), &words));
}

TEST(lexer_G, redirect_in_a_row) {
	t_list	*words;

	words = NULL;
	CHECK_EQUAL(false, lexer(ft_strdup("cat < ><>aa"), &words));
}

TEST(lexer_G, redirect_in_a_row2) {
	t_list	*words;

	words = NULL;
	CHECK_EQUAL(false, lexer(ft_strdup("cat <a><>aa"), &words));
}

TEST(lexer_G, redirect_in_a_row3) {
	t_list	*words;

	words = NULL;
	CHECK_EQUAL(false, lexer(ft_strdup("echo '<a>'<>aa"), &words));
}

TEST(lexer_G, pipe_in_a_row) {
	t_list	*words;

	words = NULL;
	CHECK_EQUAL(false, lexer(ft_strdup("echo '<a>'||aa"), &words));
}

TEST(lexer_G, pipe_in_a_row2) {
	t_list	*words;

	words = NULL;
	CHECK_EQUAL(false, lexer(ft_strdup("echo '<a>'| |aa"), &words));
}

TEST(lexer_G, pipe_in_a_row3) {
	t_list	*words;

	words = NULL;
	CHECK_EQUAL(false, lexer(ft_strdup("		| |aa"), &words));
}

TEST(lexer_G, redirect_without_target) {
	t_list	*words;

	words = NULL;
	CHECK_EQUAL(false, lexer(ft_strdup("cat input <| aa"), &words));
}

TEST(lexer_G, redirect_without_target2) {
	t_list	*words;

	words = NULL;
	CHECK_EQUAL(false, lexer(ft_strdup("cat input <0> aa"), &words));
}

TEST(lexer_G, redirect_without_target3) {
	t_list	*words;

	words = NULL;
	CHECK_EQUAL(false, lexer(ft_strdup("cat input < 0> aa"), &words));
}

// bash: syntax error near unexpected token `-1'
TEST(lexer_G, redirect_without_target4_fd_over_intmax) {
	t_list	*words;

	words = NULL;
	CHECK_EQUAL(false, lexer(ft_strdup("cat input >2147483648> aa"), &words));
}

// bash: file descriptor out of range: Bad file descriptor
TEST(lexer_G, fd_over_intmax) {
	t_list	*words;

	words = NULL;
	CHECK_EQUAL(false, lexer(ft_strdup("cat input 2147483648> aa"), &words));
}
