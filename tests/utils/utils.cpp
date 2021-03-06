#include "../test.h"

// exec_command_lineの実行結果をresult.txtに出力する
void exec_command_and_output_file(t_list *cmd_list)
{
	int bak_fd1 = dup(1);
	int bak_fd2 = dup(2);
    int file_fd1 = open("./stdout_result/result.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
	int file_fd2 = open("./stderr_result/result.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    dup2(file_fd1, 1);
	dup2(file_fd2, 2);
	g_status = exec_command_line(create_environ(g_envp), cmd_list, g_envp, ft_lstsize(cmd_list));
	close(file_fd1);
	close(file_fd2);
	dup2(bak_fd1, 1);
	dup2(bak_fd2, 2);
	close(bak_fd1);
	close(bak_fd2);
}

// exec_command_lineを実行する
void exec_command_without_dup(t_list *cmd_list)
{
	g_status = exec_command_line(create_environ(g_envp), cmd_list, g_envp, ft_lstsize(cmd_list));
}

void compare_file(
	std::string expected_filename,
	std::string actual_filename)
{
	std::ifstream actual_stream(actual_filename.c_str());
	std::string actual_string = std::string(std::istreambuf_iterator<char>(actual_stream), std::istreambuf_iterator<char>());
	std::ifstream expected_stream(expected_filename.c_str());
	std::string expected_string = std::string(std::istreambuf_iterator<char>(expected_stream), std::istreambuf_iterator<char>());

	// printf("expected:\n%s\nactual:\n%s\n", expected_string.c_str(), actual_string.c_str());
	CHECK_EQUAL(expected_string, actual_string);
}

// ビルトインコマンド単体のブロックが実行された結果をresult.txtに出力
void exec_builtin_and_output_file(t_list *cmd_list, t_environ *env)
{
	int bak_fd1 = dup(1);
	int bak_fd2 = dup(2);
    int file_fd1 = open("./stdout_result/result.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
	int file_fd2 = open("./stderr_result/result.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    dup2(file_fd1, 1);
	dup2(file_fd2, 2);
	g_status = run_builtin_command((t_cmd_block *)cmd_list->content, env);
	close(file_fd1);
	close(file_fd2);
	dup2(bak_fd1, 1);
	dup2(bak_fd2, 2);
	close(bak_fd1);
	close(bak_fd2);
}

// ビルトインコマンド単体のブロックを実行
void exec_builtin_without_dup(t_list *cmd_list, t_environ *env)
{
	printf("");
	g_status = run_builtin_only_command(cmd_list, env);
}
