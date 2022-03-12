#include "minishell.h"

unsigned char	g_status = 0;

int	main(int argc, char **argv, char **envp)
{
	char		*str;
	t_list		*words;
	t_list		*cmd_list;
	t_environ	*env;
	char		**minishell_envp;

	(void)argc;
	(void)argv;
	env = create_environ(envp);
	while (1)
	{
		words = NULL;
		cmd_list = NULL;
		str = readline("minishell$ ");
		if (ft_strlen(str) == 0)
			continue ;
		if (!lexer(str, &words) || !parser(words, &cmd_list, str))
			continue ;
		expansion(&cmd_list, env);
		if (!cmd_list)
			continue ;
		print_cmd_lst(cmd_list);
		if (is_fork_required(cmd_list))
		{
			minishell_envp = t_environ_to_vector(env);
			g_status = exec_command_line(cmd_list, minishell_envp, ft_lstsize(cmd_list));
			printf("g_status = %d\n", g_status);
		}
		else
			g_status = run_builtin_only_command(cmd_list, env);
		free(str);
	}
}
