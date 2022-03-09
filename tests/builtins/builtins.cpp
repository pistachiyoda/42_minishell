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

TEST_GROUP(builtins_G)
{
	void setup() {
    	envp_in_test = g_envp;
		remove("./builtins/out.txt");
		remove("./stdout_result/result.txt");
	}
	void teardown() {
		remove("./builtins/out.txt");
		remove("./stdout_result/result.txt");
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// pwd 正常系テストケース


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// env, export, unsert 正常系テストケース

// bash-3.2$ export hoge=test
// bash-3.2$ env | grep hoge
// hoge=test

// export hoge=test
t_list *export_hoge_test()
{
	t_cmd_block *cmd_block;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("export");
	cmd_block->args = ft_split("export hoge=test", ' ');
	cmd_block->redirects = NULL;
	return ft_lstnew(cmd_block);
}

// env | grep hoge
t_list *env_pipe_grep_hoge()
{
	t_list		*cmd_list;
	t_cmd_block *cmd_block_1;
	t_cmd_block *cmd_block_2;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_2 = (t_cmd_block *)malloc(sizeof(t_cmd_block));

	// env
	cmd_block_1->command = ft_strdup("env");
	cmd_block_1->args = ft_split("env", ' ');
	cmd_block_1->redirects = NULL;

	// grep hoge
	cmd_block_2->command = ft_strdup("grep");
	cmd_block_2->args = ft_split("grep hoge", ' ');
	cmd_block_2->redirects = NULL;

	cmd_list = ft_lstnew(cmd_block_1);
	ft_lstadd_back(&cmd_list, ft_lstnew(cmd_block_2));
	return cmd_list;
}

TEST(builtins_G, check_env_after_export)
{
	t_list *cmd_lst_1;
	t_list *cmd_lst_2;
	t_environ *env;

	env = create_environ(envp_in_test);

	cmd_lst_1 = export_hoge_test();
	cmd_lst_2 = env_pipe_grep_hoge();
	exec_builtin_and_output_file((t_cmd_block *)cmd_lst_1->content, env);
	// 出力なし
	compare_file("./builtins/expected/empty.txt");
	exec_command_and_output_file(cmd_lst_2);
	// hoge=testが出力される
	compare_file("./builtins/expected/check_env_after_export.txt");
	CHECK_EQUAL(0, g_status);

}

// bash-3.2$ date | export fuga=test2
// bash-3.2$ env | grep fuga
// bash-3.2$

// cat ./exec_cmdline/in.txt
// t_list *cat_and_arg_data()
// {
// 	t_cmd_block *cmd_block_1;

// 	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
// 	cmd_block_1->command = ft_strdup("cat");
// 	cmd_block_1->args = ft_split("cat ./exec_cmdline/in.txt", ' ');
// 	cmd_block_1->redirects = NULL;
// 	return ft_lstnew(cmd_block_1);
// }
// TEST(exec_command_line_G, cat_and_arg) {
// 	t_list *cmd_lst;

// 	cmd_lst = cat_and_arg_data();
// 	exec_command_and_output_file(cmd_lst);
// 	compare_file("in.txt");
// 	CHECK_EQUAL(0, g_status);
// }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// echo 正常系テストケース
