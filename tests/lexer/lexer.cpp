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
	expected[0] = ft_strdup("cat");
	expected[1] = ft_strdup("test.txt");
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
	expected[0] = ft_strdup("echo");
	expected[1] = ft_strdup("a");
	expected[2] = ft_strdup(">");
	expected[3] = ft_strdup("test.txt");
	expected[4] = ft_strdup(">");
	expected[5] = ft_strdup("a.out");
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
	expected[0] = ft_strdup("echo");
	expected[1] = ft_strdup("a");
	expected[2] = ft_strdup("|");
	expected[3] = ft_strdup("echo");
	expected[4] = ft_strdup("b");
	expected[5] = ft_strdup(">>");
	expected[6] = ft_strdup("c");
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
