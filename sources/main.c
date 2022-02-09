#include "minishell.h"

volatile sig_atomic_t	received_signal;

int	main(int argc, char **argv, char **envp)
{
	char	*str;
	char	**splitted_str;
	int		pid;
	int		status;

	(void)argc;
	(void)argv;
	while (1)
	{
		set_sigaction();
		str = readline("minishell$ ");
		if (ft_strlen(str) == 0)
			continue ;
		pid = fork();
		if (pid == 0)
		{
			splitted_str = ft_split(str, ' ');
			exec_command(splitted_str[0], splitted_str, envp);
			return (1);
		}
		waitpid(pid, &status, 0);
		free(str);
	}
}
