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

TEST(lexer_G, lexer_check) {
	t_list	*words;
	char	*str;
	char	**expected;
	int		i;

	// 末尾にリダイレクト以外
	str = ft_strdup("cat test.txt");
	words = lexer(str);
	expected = (char **)malloc(sizeof(char *) * 2);
	expected[0] = "cat";
	expected[1] = "test.txt";
	i = 0;
	words = words->next;
	while (words != NULL)
	{
		STRCMP_EQUAL(expected[i], (char *)(words->content));
		i++;
		words = words->next;
	}

	// 末尾にリダイレクト以外
	str = ft_strdup("echo a>test.txt>a.out");
	words = lexer(str);
	expected = (char **)malloc(sizeof(char *) * 6);
	expected[0] = "echo";
	expected[1] = "a";
	expected[2] = ">";
	expected[3] = "test.txt";
	expected[4] = ">";
	expected[5] = "a.out";
	i = 0;
	words = words->next;
	while (words != NULL)
	{
		STRCMP_EQUAL(expected[i], (char *)(words->content));
		i++;
		words = words->next;
	}

	// 末尾にリダイレクト以外
	str = ft_strdup("echo a | echo b >> c");
	words = lexer(str);
	expected = (char **)malloc(sizeof(char *) * 7);
	expected[0] = "echo";
	expected[1] = "a";
	expected[2] = "|";
	expected[3] = "echo";
	expected[4] = "b";
	expected[5] = ">>";
	expected[6] = "c";
	i = 0;
	words = words->next;
	while (words != NULL)
	{
		STRCMP_EQUAL(expected[i], (char *)(words->content));
		i++;
		words = words->next;
	}

	// リダイレクト
	str = ft_strdup("cat test.txt>");
	expected[0] = NULL;
	STRCMP_EQUAL(expected[0], (char *)lexer(str));
}
