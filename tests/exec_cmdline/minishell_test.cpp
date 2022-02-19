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

void exec_command_and_output_file(t_list *cmd_list)
{
	int bak_fd = dup(1);
    int file_fd = open("./exec_cmdline/stdout_result/result.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    dup2(file_fd, 1);
	char **envp = (char **)malloc(sizeof(char *) * 2);
	envp[0] = ft_strjoin("PATH=", getenv("PATH")); // ["PATH=xxx"]
	envp[1] = NULL;
	exec_command_line(cmd_list, envp);
	close(file_fd);
	dup2(bak_fd, 1);
	close(bak_fd);
}

void exec_command_without_dup(t_list *cmd_list)
{
	int bak_fd = dup(1);
	char **envp = (char **)malloc(sizeof(char *) * 2);
	envp[0] = ft_strjoin("PATH=", getenv("PATH")); // ["PATH=xxx"]
	envp[1] = NULL;
	exec_command_line(cmd_list, envp);
	dup2(bak_fd, 1);
	close(bak_fd);
}

void compare_file(
	std::string expected_filename,
	std::string actual_filename = "stdout_result/result.txt")
{
	std::string prefix = "./exec_cmdline/";
	std::ifstream actual_stream((prefix + actual_filename).c_str());
	std::string actual_string = std::string(std::istreambuf_iterator<char>(actual_stream), std::istreambuf_iterator<char>());
	std::ifstream expected_stream((prefix + expected_filename).c_str());
	std::string expected_string = std::string(std::istreambuf_iterator<char>(expected_stream), std::istreambuf_iterator<char>());

	CHECK_EQUAL(expected_string, actual_string);
}

TEST_GROUP(exec_command_line_G)
{
	void setup() {
		remove("./exec_cmdline/out.txt");
		remove("./exec_cmdline/stdout_result/result.txt");
	}
	void teardown() {
		remove("./exec_cmdline/out.txt");
		remove("./exec_cmdline/stdout_result/result.txt");
	}
};

// cat ./exec_cmdline/in.txt
t_list *cat_and_arg_data()
{
	t_cmd_block *cmd_block_1;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat ./exec_cmdline/in.txt", ' ');
	cmd_block_1->redirects = NULL;
	return ft_lstnew(cmd_block_1);
}
TEST(exec_command_line_G, cat_and_arg) {
	t_list *cmd_lst;

	cmd_lst = cat_and_arg_data();
	exec_command_and_output_file(cmd_lst);
	compare_file("in.txt");
}

// cat < ./exec_cmdline/in.txt
t_list *cat_with_input_redirect()
{
	t_cmd_block *cmd_block_1;
	t_redirects *input_redirect;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat", ' ');
	input_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect->redirect = INPUT;
	input_redirect->target = ft_strdup("./exec_cmdline/in.txt");
	cmd_block_1->redirects = ft_lstnew(input_redirect);
	return ft_lstnew(cmd_block_1);
}
TEST(exec_command_line_G, cat_with_input_redirect) {
	t_list *cmd_lst;

	cmd_lst = cat_with_input_redirect();
	exec_command_and_output_file(cmd_lst);
	compare_file("in.txt");
}

// cat ./exec_cmdline/in.txt > ./exec_cmdline/out.txt
t_list *cat_with_write_redirect()
{
	t_cmd_block *cmd_block_1;
	t_redirects *write_redirect;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat ./exec_cmdline/in.txt", ' ');
	write_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect->redirect = WRITE;
	write_redirect->target = ft_strdup("./exec_cmdline/out.txt");
	cmd_block_1->redirects = ft_lstnew(write_redirect);
	return ft_lstnew(cmd_block_1);
}
TEST(exec_command_line_G, cat_with_write_redirect) {
	t_list *cmd_lst;

	cmd_lst = cat_with_write_redirect();
	exec_command_without_dup(cmd_lst);
	// out.txtの比較
	compare_file("in.txt", "out.txt");
}

// cat ./exec_cmdline/in.txt >> ./exec_cmdline/out.txt
t_list *cat_with_append_redirect()
{
	t_cmd_block *cmd_block_1;
	t_redirects *input_redirect;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat ./exec_cmdline/in.txt", ' ');
	input_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect->redirect = APPEND;
	input_redirect->target = ft_strdup("./exec_cmdline/out.txt");
	cmd_block_1->redirects = ft_lstnew(input_redirect);
	return ft_lstnew(cmd_block_1);
}
TEST(exec_command_line_G, cat_with_append_redirect) {
	t_list *cmd_lst;

	cmd_lst = cat_with_append_redirect();
	exec_command_without_dup(cmd_lst);
	exec_command_without_dup(cmd_lst); // 2回実行する
	compare_file("expected/cat_with_append_redirect.txt", "out.txt");
}

// cat < ./exec_cmdline/in.txt > ./exec_cmdline/out.txt
t_list *cat_with_input_write_redirect_data()
{
	t_cmd_block *cmd_block_1;
	t_redirects *input_redirect;
	t_redirects	*write_redirect;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat", ' ');
	input_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect->redirect = INPUT;
	input_redirect->target = ft_strdup("./exec_cmdline/in.txt");
	cmd_block_1->redirects = ft_lstnew(input_redirect);
	write_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect->redirect = WRITE;
	write_redirect->target = ft_strdup("./exec_cmdline/out.txt");
	ft_lstadd_back(&cmd_block_1->redirects, ft_lstnew(write_redirect));
	return ft_lstnew(cmd_block_1);
}
TEST(exec_command_line_G, cat_with_input_write_redirect_data) {
	t_list *cmd_lst;

	cmd_lst = cat_with_input_write_redirect_data();
	exec_command_without_dup(cmd_lst);
	compare_file("in.txt", "out.txt");
}

// cat > ./exec_cmdline/out.txt < ./exec_cmdline/in.txt
t_list *cat_with_write_input_redirect_data()
{
	t_cmd_block *cmd_block_1;
	t_redirects *input_redirect;
	t_redirects	*write_redirect;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat", ' ');
	input_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect->redirect = WRITE;
	input_redirect->target = ft_strdup("./exec_cmdline/out.txt");
	cmd_block_1->redirects = ft_lstnew(input_redirect);
	write_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect->redirect = INPUT;
	write_redirect->target = ft_strdup("./exec_cmdline/in.txt");
	ft_lstadd_back(&cmd_block_1->redirects, ft_lstnew(write_redirect));
	return ft_lstnew(cmd_block_1);
}
TEST(exec_command_line_G, cat_with_write_input_redirect_data) {
	t_list *cmd_lst;

	cmd_lst = cat_with_write_input_redirect_data();
	exec_command_without_dup(cmd_lst);
	compare_file("in.txt", "out.txt");
}

// cat < ./exec_cmdline/in.txt >> ./exec_cmdline/out.txt
t_list *cat_with_input_append_redirect_data()
{
	t_cmd_block *cmd_block_1;
	t_redirects *input_redirect;
	t_redirects	*write_redirect;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat", ' ');
	input_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect->redirect = INPUT;
	input_redirect->target = ft_strdup("./exec_cmdline/in.txt");
	cmd_block_1->redirects = ft_lstnew(input_redirect);
	write_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect->redirect = APPEND;
	write_redirect->target = ft_strdup("./exec_cmdline/out.txt");
	ft_lstadd_back(&cmd_block_1->redirects, ft_lstnew(write_redirect));
	return ft_lstnew(cmd_block_1);
}
TEST(exec_command_line_G, cat_with_input_append_redirect_data) {
	t_list *cmd_lst;

	cmd_lst = cat_with_input_append_redirect_data();
	exec_command_without_dup(cmd_lst);
	exec_command_without_dup(cmd_lst); // 2回実行する
	compare_file("expected/cat_with_append_redirect.txt", "out.txt");
}

// cat >> ./exec_cmdline/out.txt < ./exec_cmdline/in.txt
t_list *cat_with_append_input_redirect_data()
{
	t_cmd_block *cmd_block_1;
	t_redirects *input_redirect;
	t_redirects	*write_redirect;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat", ' ');
	input_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect->redirect = APPEND;
	input_redirect->target = ft_strdup("./exec_cmdline/out.txt");
	cmd_block_1->redirects = ft_lstnew(input_redirect);
	write_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect->redirect = INPUT;
	write_redirect->target = ft_strdup("./exec_cmdline/in.txt");
	ft_lstadd_back(&cmd_block_1->redirects, ft_lstnew(write_redirect));
	return ft_lstnew(cmd_block_1);
}
TEST(exec_command_line_G, cat_with_append_input_redirect_data) {
	t_list *cmd_lst;

	cmd_lst = cat_with_append_input_redirect_data();
	exec_command_without_dup(cmd_lst);
	exec_command_without_dup(cmd_lst); // 2回実行する
	compare_file("expected/cat_with_append_redirect.txt", "out.txt");
}

// cat > ./exec_cmdline/out.txt < ./exec_cmdline/in.txt > ./exec_cmdline/out2.txt < ./exec_cmdline/in2.txt 
t_list *cat_with_multi_redirect1_data()
{
	t_cmd_block *cmd_block;
	t_redirects	*write_redirect;
	t_redirects *input_redirect;
	t_redirects	*write_redirect2;
	t_redirects *input_redirect2;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("cat");
	cmd_block->args = ft_split("cat", ' ');

	write_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect->redirect = WRITE;
	write_redirect->target = ft_strdup("./exec_cmdline/out.txt");
	cmd_block->redirects = ft_lstnew(write_redirect);

	input_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect->redirect = INPUT;
	input_redirect->target = ft_strdup("./exec_cmdline/in.txt");
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(input_redirect));

	write_redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect2->redirect = WRITE;
	write_redirect2->target = ft_strdup("./exec_cmdline/out2.txt");
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(write_redirect2));

	input_redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect2->redirect = INPUT;
	input_redirect2->target = ft_strdup("./exec_cmdline/in2.txt");
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(input_redirect2));
	return ft_lstnew(cmd_block);
}
TEST(exec_command_line_G, cat_with_multi_redirect_data) {
	t_list *cmd_lst;

	cmd_lst = cat_with_multi_redirect1_data();
	exec_command_without_dup(cmd_lst);
	compare_file("expected/empty.txt", "out.txt");
	compare_file("in2.txt", "out2.txt");
}

// append x 2
// マルチのリダイレクトのテストのもう一件くらい追加
// エラーケース
	// <元ファイルがない時
	// <元ファイルが読めない時
	// >, >>先ファイルが開けない時
	// コマンドが存在しない時

// cmd | cmd | cmd

// < Makefile cat | grep "make" > tmp
t_list *redirect_in_cat_and_redirect_out_grep_data()
{
	t_list		*cmd_list;
	t_cmd_block *cmd_block_1;
	t_cmd_block *cmd_block_2;
	t_redirects *input_redirect;
	t_redirects	*write_redirect;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_2 = (t_cmd_block *)malloc(sizeof(t_cmd_block));

	// < Makefile cat
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat", ' ');
	input_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect->redirect = INPUT;
	input_redirect->target = ft_strdup("test.txt");
	cmd_block_1->redirects = ft_lstnew(input_redirect);

	// grep "make" > tmp
	cmd_block_2->command = ft_strdup("grep");
	cmd_block_2->args = ft_split("grep hoge", ' ');
	write_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect->redirect = WRITE;
	write_redirect->target = ft_strdup("tmp");
	cmd_block_2->redirects = ft_lstnew(write_redirect);

	cmd_list = ft_lstnew(cmd_block_1);
	ft_lstadd_back(&cmd_list, ft_lstnew(cmd_block_2));
	return cmd_list;
}
// TEST(exec_command_line_G, redirect_in_cat_and_redirect_out_grep) {
// 	t_list *cmd_lst;

// 	printf("\n##### redirect_in_cat_and_redirect_out_grep_data #####\n");
// 	printf("##### < Makefile cat | grep \"make\" > tmp");
// 	cmd_lst = redirect_in_cat_and_redirect_out_grep_data();
// 	print_cmd_lst(cmd_lst);
// 	FAIL("Fail!");
// }

int main(int argc, char **argv)
{
	return CommandLineTestRunner::RunAllTests(argc, argv);
}




// // テストのターゲットとなる引数と返り値を決める
// void exec_command_and_output_file(t_list *cmd_list)
// {
// 	t_list *cmd_lst;
// 	char	*str;

// 	cmd_lst = parser(str); "cat test.txt"
// 	STRCMP_EQUAL("cat", ((t_cmd_block *)cmd_lst->content)->command);
// 	STRCMP_EQUAL("cat", ((t_cmd_block *)cmd_lst->content)->args[0]);
// 	STRCMP_EQUAL("test.txt", ((t_cmd_block *)cmd_lst->content)->args[1]);
	
// }
