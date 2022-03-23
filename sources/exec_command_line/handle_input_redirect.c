/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input_redirect.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 01:15:20 by fmai              #+#    #+#             */
/*   Updated: 2022/03/23 01:15:20 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_last_fd_input_redirect(
	t_redirects *redirect, t_list *redirects)
{
	t_list		*current_node;
	t_list		*last_input_node;
	t_redirects	*current_redirect;

	current_node = redirects;
	last_input_node = NULL;
	while (1)
	{
		current_redirect = current_node->content;
		if ((current_redirect->redirect == INPUT
				|| current_redirect->redirect == HEREDOC
				|| current_redirect->redirect == QUOTED_HEREDOC)
			&& current_redirect->fd == redirect->fd)
			last_input_node = current_node;
		if (!current_node->next)
			break ;
		current_node = current_node->next;
	}
	return (last_input_node->content == redirect);
}

int	handle_input(t_redirects *redirect, bool is_last)
{
	int	fd;

	if (redirect->error)
	{
		print_error(redirect->target, EMESS_AMBIGU);
		return (1);
	}
	if (!(is_exists(redirect->target)))
	{
		print_error(redirect->target, EMESS_NO_FILE_DIR);
		return (1);
	}
	if (!(is_readable(redirect->target)))
	{
		print_error(redirect->target, EMESS_NO_PERM);
		return (1);
	}
	fd = open_file(redirect->target);
	if (!is_last)
		return (0);
	if (dup2_wrapper(fd, redirect->fd) != 0)
		return (1);
	return (0);
}
