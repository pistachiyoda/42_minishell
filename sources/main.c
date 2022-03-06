#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*str;
	t_list		*words;
	t_list		*cmd_list;
	t_environ	*env;

	(void)argc;
	(void)argv;
	env = create_environ(envp);
	while (1)
	{
		cmd_list = NULL;
		str = readline("minishell$ ");
		if (ft_strlen(str) == 0)
			continue ;
		words = lexer(str);
		cmd_list = parser(words);
		if (cmd_list == NULL)
			continue ;
		print_cmd_lst(cmd_list);
		if (is_fork_required(cmd_list))
			exec_command_line(cmd_list, envp, ft_lstsize(cmd_list));
		else
			run_builtin_command(cmd_list->content, env);
		free(str);
	}
}
