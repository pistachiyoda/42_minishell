#include "minishell.h"

int		cnt_args(t_cmd_block *cmd_block)
{
	int i;

	i = 0;
	while (cmd_block->args[i])
		i ++;
	return (i);
}

bool	is_numeric(char *arg)
{
	int i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i ++;
	while (arg[i])
	{
		if (ft_isdigit(arg[i]) == 0)
			return (false);
		i ++;
	}
	return (true);
}

void	print_non_numeric_error(char *arg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("exit", 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd("numeric argument required\n", 2);
}

bool	is_bigger_than_longlong(char *arg)
{
	bool is_invalid;

	printf("nn = %lld\n", ft_atol(arg, &is_invalid));
	// if (ft_atol(arg) == (unsigned long long)LLONG_MAX + (unsigned long long)1)
	if (is_invalid)
	{
		printf("test\n");
		return (true);
	}
	return (false);
}

int		ft_exit(t_cmd_block *cmd_block)
{
	bool	is_invalid;

	if (cnt_args(cmd_block) >= 3)
	{
		print_error("exit", EMESS_TM_ARGS);
		return (1);
	}
	if ((cmd_block->args[1] != NULL && !(is_numeric(cmd_block->args[1])))
		|| (cmd_block->args[1] != NULL && is_bigger_than_longlong(cmd_block->args[1])))
	{
		ft_putstr_fd("exit\n", 2);
		print_non_numeric_error(cmd_block->args[1]);
		exit(255);
	}
	if (cmd_block->args[1] != NULL && is_numeric(cmd_block->args[1]))
	{
		ft_putstr_fd("exit\n", 2);
		exit(ft_atol(cmd_block->args[1], &is_invalid));
	}
	ft_putstr_fd("exit\n", 2);
	exit(g_status);
}
