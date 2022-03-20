#include "minishell.h"

volatile int	g_status = 0;

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q minishell");
// }

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
		set_signal(sigint_handler, SIG_IGN);
		str = readline("minishell$ ");
		if (!str)
			exit_program(g_status);
		if (ft_strlen(str) == 0)
			continue ;
		add_history(str);
		if (!lexer(str, &words) || !parser(words, &cmd_list, str))
			continue ;
		expansion(&cmd_list, env);
		// print_cmd_lst(cmd_list);
		if (!cmd_list)
			continue ;
		if (is_fork_required(cmd_list))
		{
			minishell_envp = t_environ_to_vector(env);
			g_status = exec_command_line(env, cmd_list, minishell_envp, ft_lstsize(cmd_list));
			free_2d_array(minishell_envp);
		}
		else
			g_status = run_builtin_only_command(cmd_list, env);
		free_cmd_list_after_exec(cmd_list);
		free(str);
	}
}
