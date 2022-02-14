#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*str;
	int			pid;
	int			status;
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
		// print_cmd_lst(cmd_list);
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
