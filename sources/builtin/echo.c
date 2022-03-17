#include "minishell.h"

int	handle_write_error(void)
{
	write(1, "", 0);
	if (errno == EBADF)
	{
		print_error("echo: write error", EMESS_BADF);
		return (1);
	}
	return (0);
}

bool	without_newline(t_cmd_block *cmd_block)
{
	return (cmd_block->args[1] != NULL
		&& ft_strlen(cmd_block->args[1]) == 2
		&& ft_strncmp(cmd_block->args[1], "-n", 2) == 0);
}

int	ft_echo(t_cmd_block *cmd_block)
{
	int		i;

	if (handle_write_error() != 0)
		return (1);
	if (without_newline(cmd_block))
	{
		i = 2;
		while (ft_strncmp(cmd_block->args[i], "-n", 2) == 0)
			i ++;
	}
	else
		i = 1;
	while (cmd_block->args[i])
	{
		printf("%s", cmd_block->args[i]);
		if (cmd_block->args[i + 1])
			printf(" ");
		i++;
	}
	if (!without_newline(cmd_block))
		printf("\n");
	return (0);
}
