/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_only_command.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 01:15:20 by fmai              #+#    #+#             */
/*   Updated: 2022/03/27 00:42:30 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	*dup_builtin_redirects(t_list *first_redirect)
{
	int		*fd_list;
	t_list	*current_redirect;
	int		i;

	current_redirect = first_redirect;
	fd_list = malloc(sizeof(int *) * ft_lstsize(first_redirect));
	if (fd_list == NULL)
		return (NULL);
	i = 0;
	while (current_redirect)
	{
		fd_list[i] = dup(((t_redirects *)current_redirect->content)->fd);
		i ++;
		current_redirect = current_redirect->next;
	}
	return (fd_list);
}

int	handle_dup(int *fd_list, int i, t_list *current_redirect)
{
	if (dup2(fd_list[i], ((t_redirects *)current_redirect->content)->fd)
		== -1)
	{
		perror("dup2()");
		return (1);
	}
	if (close(fd_list[i]) == -1)
	{
		perror("close()");
		return (1);
	}
	return (0);
}

int	reset_dup(t_list *first_redirect, int *fd_list)
{
	t_list	*current_redirect;
	int		i;
	int		ret;

	i = 0;
	current_redirect = first_redirect;
	while (current_redirect)
	{
		if (fd_list[i] == -1)
		{
			i ++;
			current_redirect = current_redirect->next;
			continue ;
		}
		ret = handle_dup(fd_list, i, current_redirect);
		if (ret == 1)
			return (1);
		i ++;
		current_redirect = current_redirect->next;
	}
	return (0);
}

int	free_and_return(int *fd_list, int status)
{
	free(fd_list);
	return (status);
}

int	run_builtin_only_command(t_list *cmd_list, t_environ *env)
{
	int		status;
	int		*fd_list;
	t_list	*first_redirect;

	first_redirect = ((t_cmd_block *)cmd_list->content)->redirects;
	fd_list = dup_builtin_redirects(first_redirect);
	if (fd_list == NULL)
		return (1);
	if (handle_heredoc_input(env, cmd_list) > 128)
		return (free_and_return(fd_list, 1));
	status = handle_redirects((t_cmd_block *)cmd_list->content);
	if (status != 0)
	{
		reset_dup(first_redirect, fd_list);
		return (free_and_return(fd_list, status));
	}
	status = run_builtin_command((t_cmd_block *)cmd_list->content, env);
	if (status != 0)
	{
		reset_dup(first_redirect, fd_list);
		return (free_and_return(fd_list, status));
	}
	if (reset_dup(first_redirect, fd_list) != 0)
		return (free_and_return(fd_list, 1));
	return (free_and_return(fd_list, status));
}
