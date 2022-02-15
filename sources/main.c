#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*str;
	t_list		*cmd_list;

	(void)argc;
	(void)argv;
	while (1)
	{
		cmd_list = NULL;
		str = readline("minishell$ ");
		if (ft_strlen(str) == 0)
			continue ;
		tmp_parse_data(&cmd_list, str);
		if (is_fork_required(cmd_list))
			exec_command_line(cmd_list, envp);
		else
			run_builtin_command(cmd_list->content);
		free(str);
	}
}
