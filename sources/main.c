/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasubuc <mmasubuc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 00:31:33 by fmai              #+#    #+#             */
/*   Updated: 2022/03/30 20:58:29 by mmasubuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile int	g_status = 0;

bool	is_valid_cmd_list(t_list **cmd_list, t_environ *env)
{
	char		*str;
	t_list		*words;

	words = NULL;
	set_signal(sigint_handler, SIG_IGN);
	str = readline("minishell$ ");
	if (!str)
		exit_program(g_status);
	if (ft_strlen(str) == 0)
	{
		free(str);
		return (false);
	}
	add_history(str);
	if (!lexer(str, &words) || !parser(words, cmd_list, str))
		return (false);
	expansion(cmd_list, env);
	free(str);
	return (true);
}

int	main(int argc, char **argv, char **envp)
{
	t_list		*cmd_list;
	t_environ	*env;
	char		**minishell_envp;

	(void)argc;
	(void)argv;
	env = create_environ(envp);
	while (1)
	{
		cmd_list = NULL;
		if (!is_valid_cmd_list(&cmd_list, env))
			continue ;
		if (is_fork_required(cmd_list))
		{
			minishell_envp = t_environ_to_vector(env);
			g_status = exec_command_line(
					env, cmd_list, minishell_envp, ft_lstsize(cmd_list));
			free_2d_array(minishell_envp);
		}
		else
			g_status = run_builtin_only_command(cmd_list, env);
		free_cmd_list_after_exec(cmd_list);
	}
}
