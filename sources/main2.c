#include "minishell.h"

// cmd < input > output
// cat < Makefile > out
int set_sample_data1(t_list **cmd_list)
{
	t_cmd_block *cmd_block_1;
	t_redirects *input_redirect;
	t_redirects	*write_redirect;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat", ' ');
	input_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect->redirect = INPUT;
	input_redirect->target = ft_strdup("Makefile");
	cmd_block_1->redirects = ft_lstnew(input_redirect);
	write_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect->redirect = WRITE;
	write_redirect->target = ft_strdup("out");
	ft_lstadd_back(&cmd_block_1->redirects, ft_lstnew(write_redirect));
	*cmd_list = ft_lstnew(cmd_block_1);
	return (0);
}

// cmd < input << EOF
// cat < Makefile << EOF
int set_sample_data2(t_list **cmd_list)
{
	t_cmd_block *cmd_block_1;
	t_redirects *input_redirect;
	t_redirects	*heredoc_redirect;

	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat", ' ');
	input_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	input_redirect->redirect = INPUT;
	input_redirect->target = ft_strdup("Makefile");
	cmd_block_1->redirects = ft_lstnew(input_redirect);
	heredoc_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	heredoc_redirect->redirect = WRITE;
	heredoc_redirect->target = ft_strdup("EOF");
	ft_lstadd_back(&cmd_block_1->redirects, ft_lstnew(heredoc_redirect));
	*cmd_list = ft_lstnew(cmd_block_1);
	return (0);
}

// < file1 cmd1 | cmd2 > file2
// < Makefile cat | grep "make" > tmp
int	set_sample_data3(t_list **cmd_list)
{
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
	input_redirect->target = ft_strdup("Makefile");
	cmd_block_1->redirects = ft_lstnew(input_redirect);

	// grep "make" > tmp
	cmd_block_2->command = ft_strdup("grep");
	cmd_block_2->args = ft_split("grep make", ' ');
	write_redirect = (t_redirects *)malloc(sizeof(t_redirects));
	write_redirect->redirect = WRITE;
	write_redirect->target = ft_strdup("tmp");
	cmd_block_2->redirects = ft_lstnew(write_redirect);

	*cmd_list = ft_lstnew(cmd_block_1);
	ft_lstadd_back(cmd_list, ft_lstnew(cmd_block_2));
	return (0);
}

// マルチパイプ
// cmd file | cmd | cmd
// cat Makefile | cat | grep libft
int set_sample_data4(t_list **cmd_list)
{
	t_cmd_block *cmd_block_1;
	t_cmd_block *cmd_block_2;
	t_cmd_block *cmd_block_3;
	
	cmd_block_1 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_2 = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	cmd_block_3 = (t_cmd_block *)malloc(sizeof(t_cmd_block));

	cmd_block_1->command = ft_strdup("cat");
	cmd_block_1->args = ft_split("cat Makefile", ' ');
	cmd_block_1->redirects = NULL;
	cmd_block_2->command = ft_strdup("cat");
	cmd_block_2->args = ft_split("cat", ' ');
	cmd_block_2->redirects = NULL;
	cmd_block_3->command = ft_strdup("grep");
	cmd_block_3->args = ft_split("grep libft", ' ');
	cmd_block_3->redirects = NULL;

	*cmd_list = ft_lstnew(cmd_block_1);
	ft_lstadd_back(cmd_list, ft_lstnew(cmd_block_2));
	ft_lstadd_back(cmd_list, ft_lstnew(cmd_block_3));
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char		*str;
	int			pid;
	int			status;
	int			is_error;
	t_list		*cmd_list;

	(void)argc;
	(void)argv;
	while (1)
	{
		cmd_list = NULL;
		is_error = set_sample_data1(&cmd_list);
		print_cmd_lst(cmd_list);
		str = readline("minishell$ ");
		if (ft_strlen(str) == 0)
			continue ;
		if (is_fork_required(cmd_list))
		{
			pid = fork();
			if (pid == 0)
			{
				exec_command_line(cmd_list->content, envp);
				return (1);
			}
			waitpid(pid, &status, 0);
		}
		else
			run_builtin_command(cmd_list->content);
		free(str);
	}
}
