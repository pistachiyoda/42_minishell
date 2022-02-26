#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*str;
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
		tmp_parse_data(&cmd_list, str);
		if (is_fork_required(cmd_list))
		{
			if (exec_command_line(cmd_list, envp) != 0)
				continue ;
		}
		else
			run_builtin_command(cmd_list->content, env);
		free(str);
	}
}
