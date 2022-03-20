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

t_redirects	*set_redirect(t_list **words, t_list **prev)
{
	t_redirects	*redir;
	t_list		*prev_redir;
	t_list		*next;

	redir = ft_xmalloc(sizeof(t_redirects), "parser");
	redir->fd = ft_atoi((*words)->content);
	prev_redir = *words;
	*words = (*words)->next;
	set_redirect_type(words, redir);
	next = (*words)->next;
	ft_lstdelone(*words, free);
	*words = next;
	prev_redir->next = *words;
	if ((*words)->next != NULL
		&& ft_strncmp((*words)->next->content, "|", 1) == 0)
		*prev = *words;
	redir->target = (*words)->content;
	*words = (*words)->next;
	redir->error = false;
	return (redir);
}
