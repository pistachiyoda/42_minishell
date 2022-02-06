#include "minishell.h"

int	main(void)
{
	char		*prompt;
	char		*str;
	t_cmd_bolck	*cmd_block;

	prompt = ft_strdup("$ ");
	while (1)
	{
		str = readline(prompt);
		if (ft_strlen(str) == 0)
			continue ;
		cmd_block = parse_data(str);
		run_command(cmd_block);
	}
	free(prompt);
	free(cmd_block);
}
