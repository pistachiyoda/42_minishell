/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_redirect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 23:03:08 by fmai              #+#    #+#             */
/*   Updated: 2022/03/21 23:03:08 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_redirect_type(t_list **words, t_redirects *redir)
{
	if (ft_strncmp((*words)->content, ">>", 2) == 0)
		redir->redirect = APPEND;
	else if (ft_strncmp((*words)->content, "<<", 2) == 0)
	{
		if (ft_strchr((char *)(*words)->next->content, '\'')
			|| ft_strchr((char *)(*words)->next->content, '"'))
			redir->redirect = QUOTED_HEREDOC;
		else
			redir->redirect = HEREDOC;
	}
	else if (ft_strncmp((*words)->content, ">", 1) == 0)
		redir->redirect = WRITE;
	else if (ft_strncmp((*words)->content, "<", 1) == 0)
		redir->redirect = INPUT;
}

t_redirects	*set_redirect(t_list **words)
{
	t_redirects	*redir;

	redir = ft_xmalloc(sizeof(t_redirects), "parser");
	redir->fd = ft_atoi((*words)->content);
	*words = (*words)->next;
	set_redirect_type(words, redir);
	*words = (*words)->next;
	redir->target = ft_xstrdup((*words)->content, "parser");
	*words = (*words)->next;
	redir->error = false;
	return (redir);
}
