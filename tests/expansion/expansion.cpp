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

TEST_GROUP(expansion_G)
{
	void setup() {
	}
	void teardown() {
	}
};

t_list	*get_tokens_from_expansion(char *str)
{
	t_list		*tokens;
	t_cmd_block	*cmd;
	t_redirects	*redirects;
	t_environ	*env;

	// env = create_environ(envp);
	tokens = expansion(parser(lexer(str)), env);
	cmd = (t_cmd_block *)tokens->content;
	if (cmd->redirects)
		redirects = (t_redirects *)cmd->redirects->content;
	return (tokens);
}

void	compare_tokens(t_list *tokens, t_list *exp_tokens)
{
	int			i;
	t_cmd_block	*cmd;
	t_redirects	*redirects;
	t_cmd_block	*exp_cmd;
	t_redirects	*exp_redirects;

	while (exp_tokens != NULL)
	{
		i = 0;
		cmd = (t_cmd_block *)tokens->content;
		exp_cmd = (t_cmd_block *)exp_tokens->content;
		if (exp_cmd == NULL)
			STRCMP_EQUAL((char *)exp_cmd, (char *)cmd);
		else
		{
			STRCMP_EQUAL(exp_cmd->command, cmd->command);
			while (exp_cmd->args[i] != NULL)
			{
				STRCMP_EQUAL(exp_cmd->args[i], cmd->args[i]);
				i++;
			}
		}
		while (exp_cmd != NULL && exp_cmd->redirects != NULL)
		{
			redirects = (t_redirects *)cmd->redirects->content;
			exp_redirects = (t_redirects *)exp_cmd->redirects->content;
			CHECK_EQUAL(exp_redirects->redirect, redirects->redirect);
			STRCMP_EQUAL(exp_redirects->target, redirects->target);
			CHECK_EQUAL(exp_redirects->fd, redirects->fd);
			cmd->redirects = cmd->redirects->next;
			exp_cmd->redirects = exp_cmd->redirects->next;
		}
		tokens = tokens->next;
		exp_tokens = exp_tokens->next;
	}
}

t_list	*normal(void)
{
	t_list		*exp_tokens;
	t_cmd_block	*exp_cmd;
	char		**exp_args;

	exp_cmd = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	exp_cmd->redirects = NULL;
	exp_cmd->command = ft_strdup("cat");
	exp_args = (char **)malloc(sizeof(char *) * 3);
	exp_args[0] = ft_strdup("cat");
	exp_args[1] = ft_strdup("test.txt");
	exp_args[2] = NULL;
	exp_cmd->args = exp_args;
	exp_tokens = ft_lstnew(exp_cmd);
	return (exp_tokens);
}

// TEST(expansion_G, normal) {
// 	t_list		*tokens;
// 	t_list		*exp_tokens;

// 	tokens = get_tokens_from_expansion(ft_strdup("ca't' 'test.txt'"));
// 	exp_tokens = normal();
// 	compare_tokens(tokens, exp_tokens);
// }
