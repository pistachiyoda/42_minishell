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

TEST(lexer_G, parser_check) {
	char	*str;
	bool	ret;

	// 末尾にリダイレクト以外
	str = ft_strdup("cat test.txt");
	ret = lexer(str);
	CHECK(ret);

	// リダイレクト
	str = ft_strdup("cat test.txt >");
	ret = lexer(str);
	// CHECK(ret);
	CHECK_FALSE(ret);
}
