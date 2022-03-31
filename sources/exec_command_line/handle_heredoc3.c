/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 20:47:41 by fmai              #+#    #+#             */
/*   Updated: 2022/03/31 20:48:30 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_unused_heredoc(char *str)
{
	free(str);
	exit(0);
}

void	flush_heredoc(char *str, int doc_pipe_fds[2])
{
	close_wrapper(doc_pipe_fds[0]);
	if (write(doc_pipe_fds[1], str, ft_strlen(str)) == -1)
	{
		perror("write()");
		exit(1);
	}
	close_wrapper(doc_pipe_fds[1]);
	free(str);
	exit(0);
}
