#include "minishell.h"

int	ft_echo(t_cmd_block *cmd_block)
{
	bool	without_newline;
	int		i;

	without_newline = cmd_block->args[1] != NULL
		&& ft_strlen(cmd_block->args[1]) == 2
		&& ft_strncmp(cmd_block->args[1], "-n", 2) == 0;
	if (without_newline)
		i = 2;
	else
		i = 1;
	while (cmd_block->args[i])
	{
		printf("%s", cmd_block->args[i]);
		if (cmd_block->args[i + 1])
			printf(" ");
		i++;
	}
	if (!without_newline)
		printf("\n");
	return (0);
}
