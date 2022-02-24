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
		remove("./exec_cmdline/out2.txt");
		remove("./exec_cmdline/out3.txt");
		remove("./exec_cmdline/out4.txt");
		remove("./exec_cmdline/stdout_result/result.txt");
	}
	void teardown() {
		remove("./exec_cmdline/out.txt");
		remove("./exec_cmdline/out2.txt");
		remove("./exec_cmdline/out3.txt");
		remove("./exec_cmdline/out4.txt");
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
	input_redirect->fd = 0;
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
	write_redirect->fd = 1;
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
	t_redirects *append_redirect;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat ./exec_cmdline/in.txt", ' ');
	append_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	append_redirect->redirect = APPEND;
	append_redirect->target = ft_strdup("./exec_cmdline/out.txt");
	append_redirect->fd = 1;
	cmd_block_1->redirects = ft_lstnew(append_redirect);
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
	input_redirect->fd = 0;
	cmd_block_1->redirects = ft_lstnew(input_redirect);
	write_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect->redirect = WRITE;
	write_redirect->target = ft_strdup("./exec_cmdline/out.txt");
	write_redirect->fd = 1;
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
	t_redirects	*write_redirect;
	t_redirects *input_redirect;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat", ' ');
	write_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect->redirect = WRITE;
	write_redirect->fd = 1;
	write_redirect->target = ft_strdup("./exec_cmdline/out.txt");
	cmd_block_1->redirects = ft_lstnew(write_redirect);
	input_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect->redirect = INPUT;
	input_redirect->target = ft_strdup("./exec_cmdline/in.txt");
	input_redirect->fd = 0;
	ft_lstadd_back(&cmd_block_1->redirects, ft_lstnew(input_redirect));
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
	t_redirects	*append_redirect;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat", ' ');
	input_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect->redirect = INPUT;
	input_redirect->target = ft_strdup("./exec_cmdline/in.txt");
	input_redirect->fd = 0;
	cmd_block_1->redirects = ft_lstnew(input_redirect);
	append_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	append_redirect->redirect = APPEND;
	append_redirect->target = ft_strdup("./exec_cmdline/out.txt");
	append_redirect->fd = 1;
	ft_lstadd_back(&cmd_block_1->redirects, ft_lstnew(append_redirect));
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
	t_redirects *append_redirect;
	t_redirects	*input_redirect;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat", ' ');
	append_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	append_redirect->redirect = APPEND;
	append_redirect->target = ft_strdup("./exec_cmdline/out.txt");
	append_redirect->fd = 1;
	cmd_block_1->redirects = ft_lstnew(append_redirect);
	input_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect->redirect = INPUT;
	input_redirect->target = ft_strdup("./exec_cmdline/in.txt");
	input_redirect->fd = 0;
	ft_lstadd_back(&cmd_block_1->redirects, ft_lstnew(input_redirect));
	return ft_lstnew(cmd_block_1);
}
TEST(exec_command_line_G, cat_with_append_input_redirect_data) {
	t_list *cmd_lst;

	cmd_lst = cat_with_append_input_redirect_data();
	exec_command_without_dup(cmd_lst);
	exec_command_without_dup(cmd_lst); // 2回実行する
	compare_file("expected/cat_with_append_redirect.txt", "out.txt");
}

// cat ./exec_cmdline/in.txt >> ./exec_cmdline/out.txt >> ./exec_cmdline/out2.txt
t_list *cat_with_2append_1_redirect_data()
{
	t_cmd_block *cmd_block_1;
	t_redirects *append_redirect1;
	t_redirects	*append_redirect2;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat ./exec_cmdline/in.txt", ' ');
	append_redirect1 = (t_redirects *)malloc(sizeof(t_redirects));
	append_redirect1->redirect = APPEND;
	append_redirect1->target = ft_strdup("./exec_cmdline/out.txt");
	append_redirect1->fd = 1;
	cmd_block_1->redirects = ft_lstnew(append_redirect1);
	append_redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	append_redirect2->redirect = APPEND;
	append_redirect2->target = ft_strdup("./exec_cmdline/out2.txt");
	append_redirect2->fd = 1;
	ft_lstadd_back(&cmd_block_1->redirects, ft_lstnew(append_redirect2));
	return ft_lstnew(cmd_block_1);
}
TEST(exec_command_line_G, cat_with_2append_1_redirect_data) {
	t_list *cmd_lst;

	cmd_lst = cat_with_2append_1_redirect_data();
	exec_command_without_dup(cmd_lst);
	// 一つ目のファイルは空
	compare_file("expected/empty.txt", "out.txt");
	// ２つ目のファイルはin.txtと同様の内容が記入される
	compare_file("expected/cat_with_2append_1_redirect_data.txt", "out2.txt");
}

// cat ./exec_cmdline/in.txt >> ./exec_cmdline/out.txt >> ./exec_cmdline/out.txt
t_list *cat_with_2append_2_redirect_data()
{
	t_cmd_block *cmd_block_1;
	t_redirects *append_redirect1;
	t_redirects	*append_redirect2;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat ./exec_cmdline/in.txt", ' ');
	append_redirect1 = (t_redirects *)malloc(sizeof(t_redirects));
	append_redirect1->redirect = APPEND;
	append_redirect1->target = ft_strdup("./exec_cmdline/out.txt");
	append_redirect1->fd = 1;
	cmd_block_1->redirects = ft_lstnew(append_redirect1);
	append_redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	append_redirect2->redirect = APPEND;
	append_redirect2->target = ft_strdup("./exec_cmdline/out.txt");
	append_redirect2->fd = 1;
	ft_lstadd_back(&cmd_block_1->redirects, ft_lstnew(append_redirect2));
	return ft_lstnew(cmd_block_1);
}
TEST(exec_command_line_G, cat_with_2append_2_redirect_data) {
	t_list *cmd_lst;

	cmd_lst = cat_with_2append_2_redirect_data();
	exec_command_without_dup(cmd_lst);
	// 追記は最後のものしか実行されないため、in.txtと同様の内容が記入される
	compare_file("expected/cat_with_2append_2_redirect_data.txt", "out.txt");
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
	write_redirect->fd = 1;
	cmd_block->redirects = ft_lstnew(write_redirect);

	input_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect->redirect = INPUT;
	input_redirect->target = ft_strdup("./exec_cmdline/in.txt");
	input_redirect->fd = 0;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(input_redirect));

	write_redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect2->redirect = WRITE;
	write_redirect2->target = ft_strdup("./exec_cmdline/out2.txt");
	write_redirect2->fd = 1;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(write_redirect2));

	input_redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect2->redirect = INPUT;
	input_redirect2->target = ft_strdup("./exec_cmdline/in2.txt");
	input_redirect2->fd = 0;
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

// ls > ./exec_cmdline/out.txt > ./exec_cmdline/out2.txt > ./exec_cmdline/out3.txt > ./exec_cmdline/out4.txt
// ls > ./exec_cmdline/expected/cat_with_multi_redirect2_data_out.txt > ./exec_cmdline/expected/cat_with_multi_redirect2_data_out2.txt > ./exec_cmdline/expected/cat_with_multi_redirect2_data_out3.txt > ./exec_cmdline/expected/cat_with_multi_redirect2_data_out4.txt
t_list *cat_with_multi_redirect2_data()
{
	t_cmd_block *cmd_block;
	t_redirects	*write_redirect1;
	t_redirects *write_redirect2;
	t_redirects	*write_redirect3;
	t_redirects *write_redirect4;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("ls");
	cmd_block->args = ft_split("ls", ' ');

	write_redirect1 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect1->redirect = WRITE;
	write_redirect1->target = ft_strdup("./exec_cmdline/out.txt");
	write_redirect1->fd = 1;
	cmd_block->redirects = ft_lstnew(write_redirect1);

	write_redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect2->redirect = WRITE;
	write_redirect2->target = ft_strdup("./exec_cmdline/out2.txt");
	write_redirect2->fd = 1;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(write_redirect2));

	write_redirect3 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect3->redirect = WRITE;
	write_redirect3->target = ft_strdup("./exec_cmdline/out3.txt");
	write_redirect3->fd = 1;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(write_redirect3));

	write_redirect4 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect4->redirect = WRITE;
	write_redirect4->target = ft_strdup("./exec_cmdline/out4.txt");
	write_redirect4->fd = 1;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(write_redirect4));
	return ft_lstnew(cmd_block);
}
TEST(exec_command_line_G, cat_with_multi_redirect2_data) {
	t_list *cmd_lst;

	cmd_lst = cat_with_multi_redirect2_data();
	exec_command_without_dup(cmd_lst);
	// 最後のwriteリダイレクトのみ実行される
	compare_file("expected/empty.txt", "out.txt");
	compare_file("expected/empty.txt", "out2.txt");
	compare_file("expected/empty.txt", "out3.txt");
	compare_file("expected/cat_with_multi_redirect2_data_out4.txt", "out4.txt");
}

// 2> out.txt
t_list	*fd_redirect_and_file_only_data()
{
	t_cmd_block	*cmd_block;
	t_redirects	*write_redirect;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = NULL;
	cmd_block->args = NULL;
	write_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect->redirect = WRITE;
	write_redirect->target = ft_strdup("./exec_cmdline/out.txt");
	write_redirect->fd = 2;
	cmd_block->redirects = ft_lstnew(write_redirect);
	return ft_lstnew(cmd_block);
}
TEST(exec_command_line_G, fd_redirect_and_file_only_data)
{
	t_list	*cmd_list;

	cmd_list = fd_redirect_and_file_only_data();
	exec_command_and_output_file(cmd_list);
	// コマンドが実行されなくてもファイルは作成される
	compare_file("expected/empty.txt", "out.txt");
}
// 100<in.txt cat in.txt
t_list	*unrelated_fd_redirect_data()
{
	t_cmd_block	*cmd_block;
	t_redirects	*input_redirect;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("cat");
	cmd_block->args = ft_split("cat in.txt", ' ');
	input_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect->redirect = INPUT;
	input_redirect->target = ft_strdup("./exec_cmdline/in.txt");
	input_redirect->fd = 100;
	cmd_block->redirects = ft_lstnew(input_redirect);
	return ft_lstnew(cmd_block);
}
TEST(exec_command_line_G, unrelated_fd_redirect)
{
	t_list	*cmd_list;

	cmd_list = unrelated_fd_redirect_data();
	exec_command_and_output_file(cmd_list);
	// 特に何も起こらない
	compare_file("in.txt");
}

// 100>out.txt cat in.txt
t_list	*unrelated_fd_redirect_with_cat_data()
{
	t_cmd_block	*cmd_block;
	t_redirects	*write_redirect;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("cat");
	cmd_block->args = ft_split("cat in.txt", ' ');
	write_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect->redirect = WRITE;
	write_redirect->target = ft_strdup("./exec_cmdline/in.txt");
	write_redirect->fd = 100;
	cmd_block->redirects = ft_lstnew(write_redirect);
	return ft_lstnew(cmd_block);
}
TEST(exec_command_line_G, unrelated_fd_redirect_with_cat)
{
	t_list	*cmd_list;

	cmd_list = unrelated_fd_redirect_with_cat_data();
	exec_command_and_output_file(cmd_list);
	// 何も書かれていないout.txtが作成され、result.txt(標準出力)にin.txtの内容が出力される
	compare_file("expected/empty.txt", "out.txt");
	compare_file("in.txt");
}

// 100> out.txt echo hoge
// 100> ./exec_cmdline/expected/unrelated_fd_redirect_with_echo_out.txt echo hoge
t_list	*unrelated_fd_redirect_with_echo_data()
{
	t_cmd_block	*cmd_block;
	t_redirects	*write_redirect;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("echo");
	cmd_block->args = ft_split("echo hoge", ' ');
	write_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect->redirect = WRITE;
	write_redirect->target = ft_strdup("./exec_cmdline/out.txt");
	write_redirect->fd = 100;
	cmd_block->redirects = ft_lstnew(write_redirect);
	return ft_lstnew(cmd_block);
}
TEST(exec_command_line_G, unrelated_fd_redirect_with_echo)
{
	t_list	*cmd_list;

	cmd_list = unrelated_fd_redirect_with_echo_data();
	exec_command_and_output_file(cmd_list);
	// 何も書かれていないout.txtが作成され、result.txt(標準出力)にin3.txt(hoge)の内容が出力される
	compare_file("expected/empty.txt", "out.txt");
	compare_file("in3.txt");
}

// 100> out1.txt 101> out2.txt echo hoge
// 100> ./exec_cmdline/expected/two_unrelated_fd_write_redirect_with_echo_out1.txt 101> ./exec_cmdline/expected/two_unrelated_fd_write_redirect_with_echo_out2.txt echo hoge
t_list	*two_unrelated_fd_write_redirect_with_echo_data()
{
	t_cmd_block	*cmd_block;
	t_redirects	*write_redirect1;
	t_redirects	*write_redirect2;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("echo");
	cmd_block->args = ft_split("echo hoge", ' ');

	write_redirect1 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect1->redirect = WRITE;
	write_redirect1->target = ft_strdup("./exec_cmdline/out.txt");
	write_redirect1->fd = 100;
	cmd_block->redirects = ft_lstnew(write_redirect1);

	write_redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect2->redirect = WRITE;
	write_redirect2->target = ft_strdup("./exec_cmdline/out2.txt");
	write_redirect2->fd = 101;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(write_redirect2));

	return ft_lstnew(cmd_block);
}
TEST(exec_command_line_G, two_unrelated_fd_write_redirect_with_echo)
{
	t_list	*cmd_list;

	cmd_list = two_unrelated_fd_write_redirect_with_echo_data();
	exec_command_and_output_file(cmd_list);
	// 何も書かれていないout1.txt, out2.txtが作成される
	compare_file("expected/empty.txt", "out1.txt");
	compare_file("expected/empty.txt", "out2.txt");
	// result.txt(標準出力)にin3.txt(hoge)の内容が出力される
	compare_file("in3.txt");
}

// 100> out1.txt 101> out2.txt 1> out3.txt echo hoge
// 100> ./exec_cmdline/expected/two_unrelated_fd_and_1_write_with_echo_out1.txt 101> ./exec_cmdline/expected/two_unrelated_fd_and_1_write_with_echo_out2.txt 1> ./exec_cmdline/expected/two_unrelated_fd_and_1_write_with_echo_out3.txt echo hoge
t_list	*two_unrelated_fd_and_1_write_with_echo_data()
{
	t_cmd_block	*cmd_block;
	t_redirects	*write_redirect1;
	t_redirects	*write_redirect2;
	t_redirects	*write_redirect3;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("echo");
	cmd_block->args = ft_split("echo hoge", ' ');

	write_redirect1 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect1->redirect = WRITE;
	write_redirect1->target = ft_strdup("./exec_cmdline/out.txt");
	write_redirect1->fd = 100;
	cmd_block->redirects = ft_lstnew(write_redirect1);

	write_redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect2->redirect = WRITE;
	write_redirect2->target = ft_strdup("./exec_cmdline/out2.txt");
	write_redirect2->fd = 101;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(write_redirect2));

	write_redirect3 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect3->redirect = WRITE;
	write_redirect3->target = ft_strdup("./exec_cmdline/out3.txt");
	write_redirect3->fd = 1;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(write_redirect3));

	return ft_lstnew(cmd_block);
}
TEST(exec_command_line_G, two_unrelated_fd_and_1_write_with_echo)
{
	t_list	*cmd_list;

	cmd_list = two_unrelated_fd_and_1_write_with_echo_data();
	exec_command_and_output_file(cmd_list);
	// 何も書かれていないout1.txt, out2.txtが作成される
	compare_file("expected/empty.txt", "out1.txt");
	compare_file("expected/empty.txt", "out2.txt");
	// result.txt(標準出力)にin3.txt(hoge)の内容が出力される
	compare_file("in3.txt");
}

// 1> out1.txt 2> out2.txt echo hoge
// 1> ./exec_cmdline/expected/two_fd_write_redirect_with_echo_out1.txt 2> ./exec_cmdline/expected/two_fd_write_redirect_with_echo_out2.txt echo hoge
t_list *two_fd_write_redirect_with_echo_date()
{
	t_cmd_block *cmd_block;
	t_redirects	*write_redirect1;
	t_redirects	*write_redirect2;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("echo");
	cmd_block->args = ft_split("echo hoge", ' ');

	write_redirect1 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect1->redirect = WRITE;
	write_redirect1->target = ft_strdup("./exec_cmdline/out1.txt");
	write_redirect1->fd = 1;
	cmd_block->redirects = ft_lstnew(write_redirect1);

	write_redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect2->redirect = WRITE;
	write_redirect2->target = ft_strdup("./exec_cmdline/out2.txt");
	write_redirect2->fd = 2;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(write_redirect2));

	return ft_lstnew(cmd_block);
}
TEST(exec_command_line_G, two_fd_write_redirect_with_echo_date) {
	t_list *cmd_lst;

	cmd_lst = two_fd_write_redirect_with_echo_date();
	exec_command_without_dup(cmd_lst);
	// echoから1に書き込まれる。
	compare_file("expected/two_fd_write_redirect_with_echo_out1.txt", "out1.txt");
	// エラー出力（2）は何も受け取らないので空
	compare_file("expected/empty.txt", "out2.txt");
}

// 2> out1.txt 1> out2.txt echo hoge
// 2> ./exec_cmdline/expected/two_fd_write_redirect_with_echo2_out1.txt 1> ./exec_cmdline/expected/two_fd_write_redirect_with_echo2_out2.txt echo hoge
t_list *two_fd_write_redirect_with_echo2_date()
{
	t_cmd_block *cmd_block;
	t_redirects	*write_redirect1;
	t_redirects	*write_redirect2;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("echo");
	cmd_block->args = ft_split("echo hoge", ' ');

	write_redirect1 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect1->redirect = WRITE;
	write_redirect1->target = ft_strdup("./exec_cmdline/out1.txt");
	write_redirect1->fd = 2;
	cmd_block->redirects = ft_lstnew(write_redirect1);

	write_redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect2->redirect = WRITE;
	write_redirect2->target = ft_strdup("./exec_cmdline/out2.txt");
	write_redirect2->fd = 1;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(write_redirect2));

	return ft_lstnew(cmd_block);
}
TEST(exec_command_line_G, two_fd_write_redirect_with_echo2_date) {
	t_list *cmd_lst;

	cmd_lst = two_fd_write_redirect_with_echo2_date();
	exec_command_without_dup(cmd_lst);
	// エラー出力（2）は何も受け取らないのでout1.txtは空
	compare_file("expected/empty.txt", "out1.txt");
	// echoから1に書き込まれる。
	compare_file("expected/two_fd_write_redirect_with_echo2_out2.txt", "out2.txt");
}

// 1> out1.txt 2> out2.txt 1>out3.txt echo hoge
// 1> ./exec_cmdline/expected/three_fd_write_redirect_with_echo_out1.txt 2> ./exec_cmdline/expected/three_fd_write_redirect_with_echo_out2.txt 1> ./exec_cmdline/expected/three_fd_write_redirect_with_echo_out3.txt echo hoge
t_list *three_fd_write_redirect_with_echo_date()
{
	t_cmd_block *cmd_block;
	t_redirects	*write_redirect1;
	t_redirects	*write_redirect2;
	t_redirects	*write_redirect3;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("echo");
	cmd_block->args = ft_split("echo hoge", ' ');

	write_redirect1 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect1->redirect = WRITE;
	write_redirect1->target = ft_strdup("./exec_cmdline/out1.txt");
	write_redirect1->fd = 1;
	cmd_block->redirects = ft_lstnew(write_redirect1);

	write_redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect2->redirect = WRITE;
	write_redirect2->target = ft_strdup("./exec_cmdline/out2.txt");
	write_redirect2->fd = 2;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(write_redirect2));

	write_redirect3 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect3->redirect = WRITE;
	write_redirect3->target = ft_strdup("./exec_cmdline/out3.txt");
	write_redirect3->fd = 1;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(write_redirect2));

	cmd_block->redirects = ft_lstnew(write_redirect3);

	return ft_lstnew(cmd_block);
}
TEST(exec_command_line_G, three_fd_write_redirect_with_echo_date) {
	t_list *cmd_lst;

	cmd_lst = three_fd_write_redirect_with_echo_date();
	exec_command_without_dup(cmd_lst);
	// 最初の1は処理されないのでout1.txtは空
	compare_file("expected/empty.txt", "out1.txt");
	// エラー出力（2）は何も受け取らないのでout2.txtは空
	compare_file("expected/empty.txt", "out2.txt");
	// echoから最後の1にのみ書き込まれる。
	compare_file("expected/three_fd_write_redirect_with_echo_out3.txt", "out3.txt");
}

// 1> out1.txt 2> out2.txt dog
// 1> ./exec_cmdline/expected/two_fd_write_redirect_with_dog_out1.txt 2> ./exec_cmdline/expected/two_fd_write_redirect_with_dog_out2.txt dog
t_list *two_fd_write_redirect_with_dog_date()
{
	t_cmd_block *cmd_block;
	t_redirects	*write_redirect1;
	t_redirects	*write_redirect2;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("dog");
	cmd_block->args = ft_split("dog", ' ');

	write_redirect1 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect1->redirect = WRITE;
	write_redirect1->target = ft_strdup("./exec_cmdline/out1.txt");
	write_redirect1->fd = 1;
	cmd_block->redirects = ft_lstnew(write_redirect1);

	write_redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect2->redirect = WRITE;
	write_redirect2->target = ft_strdup("./exec_cmdline/out2.txt");
	write_redirect2->fd = 2;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(write_redirect2));

	return ft_lstnew(cmd_block);
}
TEST(exec_command_line_G, two_fd_write_redirect_with_dog_date) {
	t_list *cmd_lst;

	cmd_lst = two_fd_write_redirect_with_dog_date();
	exec_command_without_dup(cmd_lst);
	// 標準出力（1）は何も受け取らないのでout1.txtは空
	compare_file("expected/empty.txt", "out1.txt");
	// dogコマンドのエラー出力が2に書き込まれる。
	compare_file("expected/two_fd_write_redirect_with_dog_out2.txt", "out2.txt");
}

// 2> out1.txt 1> out2.txt 2> out3.txt dog
// 2> ./exec_cmdline/expected/three_fd_write_redirect_with_dog_out1.txt 1> ./exec_cmdline/expected/three_fd_write_redirect_with_dog_out2.txt 2> ./exec_cmdline/expected/three_fd_write_redirect_with_dog_out3.txt dog
t_list *three_fd_write_redirect_with_dog_date()
{
	t_cmd_block *cmd_block;
	t_redirects	*write_redirect1;
	t_redirects	*write_redirect2;
	t_redirects	*write_redirect3;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("dog");
	cmd_block->args = ft_split("dog", ' ');

	write_redirect1 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect1->redirect = WRITE;
	write_redirect1->target = ft_strdup("./exec_cmdline/out1.txt");
	write_redirect1->fd = 2;
	cmd_block->redirects = ft_lstnew(write_redirect1);

	write_redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect2->redirect = WRITE;
	write_redirect2->target = ft_strdup("./exec_cmdline/out2.txt");
	write_redirect2->fd = 1;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(write_redirect2));

	write_redirect3 = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect3->redirect = WRITE;
	write_redirect3->target = ft_strdup("./exec_cmdline/out3.txt");
	write_redirect3->fd = 2;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(write_redirect2));

	cmd_block->redirects = ft_lstnew(write_redirect3);

	return ft_lstnew(cmd_block);
}
TEST(exec_command_line_G, three_fd_write_redirect_with_dog_date) {
	t_list *cmd_lst;

	cmd_lst = three_fd_write_redirect_with_dog_date();
	exec_command_without_dup(cmd_lst);
	// 最初の2は処理されないのでout1.txtは空
	compare_file("expected/empty.txt", "out1.txt");
	// 標準出力（1）は何も受け取らないのでout2.txtは空
	compare_file("expected/empty.txt", "out2.txt");
	// echoから最後の2にのみ書き込まれる。
	compare_file("expected/three_fd_write_redirect_with_dog_out3.txt", "out3.txt");
}

// 0< in1.txt cat 100< in2.txt
// 0< ./exec_cmdline/in.txt cat 100< ./exec_cmdline/in2.txt
t_list *zero_100_input_redirect_with_cat_date()
{
	t_cmd_block *cmd_block;
	t_redirects	*input_redirect1;
	t_redirects	*input_redirect2;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("cat");
	cmd_block->args = ft_split("cat", ' ');

	input_redirect1 = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect1->redirect = INPUT;
	input_redirect1->target = ft_strdup("./exec_cmdline/in.txt");
	input_redirect1->fd = 0;
	cmd_block->redirects = ft_lstnew(input_redirect1);

	input_redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect2->redirect = INPUT;
	input_redirect2->target = ft_strdup("./exec_cmdline/in2.txt");
	input_redirect2->fd = 100;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(input_redirect2));

	return ft_lstnew(cmd_block);
}
TEST(exec_command_line_G, zero_100_input_redirect_with_cat_date) {
	t_list *cmd_lst;

	cmd_lst = zero_100_input_redirect_with_cat_date();
	exec_command_without_dup(cmd_lst);
	// 標準出力（1）にin.txtの内容が出力されるので、result.txtと比較
	compare_file("in.txt");
}

// 0<　in1.txt cat 0<　in2.txt
// 0< ./exec_cmdline/in.txt cat 0< ./exec_cmdline/in2.txt
t_list *two_fd_input_redirect_with_cat_date()
{
	t_cmd_block *cmd_block;
	t_redirects	*input_redirect1;
	t_redirects	*input_redirect2;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block->command = ft_strdup("cat");
	cmd_block->args = ft_split("cat", ' ');

	input_redirect1 = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect1->redirect = INPUT;
	input_redirect1->target = ft_strdup("./exec_cmdline/in.txt");
	input_redirect1->fd = 0;
	cmd_block->redirects = ft_lstnew(input_redirect1);

	input_redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect2->redirect = INPUT;
	input_redirect2->target = ft_strdup("./exec_cmdline/in2.txt");
	input_redirect2->fd = 0;
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(input_redirect2));

	return ft_lstnew(cmd_block);
}
TEST(exec_command_line_G, two_fd_input_redirect_with_cat_date) {
	t_list *cmd_lst;

	cmd_lst = two_fd_input_redirect_with_cat_date();
	exec_command_without_dup(cmd_lst);
	// 後半の0<が実行され、標準出力（1）にin2.txtの内容が出力されるので、result.txtと比較
	compare_file("in2.txt");
}

//  2> out.txt 1< in.txt echo hoge
// エラーケースなので正常系実装した後にエラーケースでまとめてコミットする
// waitpidのstatusでエラー判定するような感じで実装。
// t_list	*invalid_stdout_redirect_input_data()
// {
// 	t_cmd_block *cmd_block_1;
// 	t_redirects	*write_redirect;
// 	t_redirects *input_redirect;

// 	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
// 	cmd_block_1->command = ft_strdup("echo");
// 	cmd_block_1->args = ft_split("echo hoge", ' ');
// 	write_redirect = (t_redirects *)malloc(sizeof(t_redirects));
// 	write_redirect->redirect = WRITE;
// 	write_redirect->fd = 2;
// 	write_redirect->target = ft_strdup("./exec_cmdline/out.txt");
// 	cmd_block_1->redirects = ft_lstnew(write_redirect);
// 	input_redirect = (t_redirects *)malloc(sizeof(t_redirects));
// 	input_redirect->redirect = INPUT;
// 	input_redirect->target = ft_strdup("./exec_cmdline/in.txt");
// 	input_redirect->fd = 1;
// 	ft_lstadd_back(&cmd_block_1->redirects, ft_lstnew(input_redirect));
// 	return ft_lstnew(cmd_block_1);
// }
// TEST(exec_command_line_G, invalid_stdout_redirect_input_data) {
// 	t_list *cmd_lst;

// 	cmd_lst = invalid_stdout_redirect_input_data();
// 	exec_command_without_dup(cmd_lst);
// 	// echoを実行するが、1< in.txtで標準出力(1)がcloseされるため、エラーになり、
// 	// out.txtにエラー文が出力される。
// 	compare_file("expected/invalid_stdout_redirect_input.txt", "out.txt");
// }

// 手動で！
// 2< in.txt dog

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
