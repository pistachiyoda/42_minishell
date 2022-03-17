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

TEST_GROUP(pwd_G)
{
	void setup() {
		system("pwd > ./builtin/expected/pwd_only.txt");
		// remove("./builtins/out.txt");
		// remove("./stdout_result/result.txt");
	}
	void teardown() {
		// remove("./builtins/out.txt");
		// remove("./stdout_result/result.txt");
		remove("./builtin/expected/pwd_only.txt");
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// pwd 正常系テストケース

// pwd
t_list *pwd_only()
{
	t_cmd_block *cmd_block;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("pwd");
	cmd_block->args = ft_split("pwd", ' ');
	cmd_block->redirects = NULL;
	return ft_lstnew(cmd_block);
}
TEST(pwd_G, pwd_only) {
	t_list *cmd_lst;
	t_environ *env;

	system("pwd > ./builtin/expected/pwd_only.txt");
	cmd_lst = pwd_only();
	env = create_environ(g_envp);

	exec_builtin_and_output_file(cmd_lst, env);

	compare_file("./builtin/expected/pwd_only.txt");
	CHECK_EQUAL(0, g_status);
}

// pwd > out.txt
t_list *pwd_and_write()
{
	t_cmd_block *cmd_block;
	t_redirects *write_redirect;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("pwd");
	cmd_block->args = ft_split("pwd", ' ');

	write_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect->redirect = WRITE;
	write_redirect->target = ft_strdup("./builtin/out.txt");
	write_redirect->fd = 1;
	cmd_block->redirects = ft_lstnew(write_redirect);

	return ft_lstnew(cmd_block);
}
TEST(pwd_G, pwd_and_write) {
	t_list *cmd_lst;
	t_environ *env;

	cmd_lst = pwd_and_write();
	env = create_environ(g_envp);
	exec_builtin_without_dup(cmd_lst, env);
	compare_file("./builtin/expected/pwd_only.txt", "./builtin/out.txt");
	CHECK_EQUAL(0, g_status);
}

