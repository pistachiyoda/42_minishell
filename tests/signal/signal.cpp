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

TEST_GROUP(signal_G)
{
	void setup() {
	}
	void teardown() {
	}
};

t_list	*exit_status_after_signal(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("echo");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("echo");
	exp_args[1] = ft_strdup("0");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

TEST(signal_G, exit_status_after_signal) {
	t_list		*tokens;
	t_list		*exp_tokens;

	tokens = get_tokens_from_expansion(ft_strdup("echo $?"), create_environ(g_envp));
	exp_tokens = exit_status_after_signal();
	compare_tokens(tokens, exp_tokens);
	g_status = 0;
}
