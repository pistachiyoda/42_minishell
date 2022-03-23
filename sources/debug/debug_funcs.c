/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_funcs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 21:22:59 by fmai              #+#    #+#             */
/*   Updated: 2022/03/22 21:22:59 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
void	print_string_array(char **string_array, char *key)
{
	int	i;

	i = 0;
	while (string_array[i])
	{
		printf("%s[%d]: %s\n", key, i, string_array[i]);
		i++;
	}
	return ;
}

char	*print_redirect(enum e_REDIRECTS redirect)
{
	if (redirect == WRITE)
		return (">");
	if (redirect == APPEND)
		return (">>");
	if (redirect == INPUT)
		return ("<");
	if (redirect == HEREDOC)
		return ("<<");
	if (redirect == QUOTED_HEREDOC)
		return ("'<<'");
	return ("not_redirection");
}

void	print_list(t_list *lst)
{
	int	i;

	i = 0;
	while (1)
	{
		printf("++++ redirect %d++++\n", i);
		printf("redirect: %s\n",
			print_redirect(((t_redirects *)lst->content)->redirect));
		printf("target: %s\n", ((t_redirects *)lst->content)->target);
		printf("fd: %d\n", ((t_redirects *)lst->content)->fd);
		if (lst->next == NULL)
			return ;
		lst = lst->next;
		i++;
	}
}

void	print_cmd_lst(t_list *cmd_lst)
{
	int	i;

	i = 0;
	while (1)
	{
		printf("\n====== block %d ======\n", i);
		printf("cmd: %s\n", ((t_cmd_block *)cmd_lst->content)->command);
		if (((t_cmd_block *)cmd_lst->content)->args)
			print_string_array(((t_cmd_block *)cmd_lst->content)->args, "args");
		if (((t_cmd_block *)cmd_lst->content)->redirects)
			print_list(((t_cmd_block *)cmd_lst->content)->redirects);
		printf("=====================\n");
		if (cmd_lst->next == NULL)
			break ;
		cmd_lst = cmd_lst->next;
		i++;
	}
}

void	print_leaks(const char *text)
{
	printf("========%s===========\n", text);
	system("leaks -q minishell 2>&1 | grep 'leaks for'");
}
