/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 13:36:58 by fmai              #+#    #+#             */
/*   Updated: 2022/03/23 12:05:35 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cnt_args(t_cmd_block *cmd_block)
{
	int	i;

	i = 0;
	while (cmd_block->args[i])
		i ++;
	return (i);
}

bool	is_numeric(char *arg)
{
	int	i;

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

int	ft_exit(t_cmd_block *cmd_block)
{
	bool		is_invalid;
	char		*first_arg;
	long long	status;

	first_arg = cmd_block->args[1];
	if (first_arg == NULL)
	{
		ft_putstr_fd("exit\n", 2);
		exit(g_status);
	}
	if (cnt_args(cmd_block) >= 3 && is_numeric(first_arg))
	{
		print_error("exit", EMESS_TM_ARGS);
		return (1);
	}
	status = ft_atol(first_arg, &is_invalid);
	if (!is_numeric(first_arg) || is_invalid)
	{
		ft_putstr_fd("exit\n", 2);
		print_non_numeric_error(first_arg);
		exit(255);
	}
	ft_putstr_fd("exit\n", 2);
	exit(status);
}
