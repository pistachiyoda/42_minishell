#include "minishell.h"

bool	is_last_input_redirect(
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
				|| current_redirect->redirect == HEREDOC)
			&& current_redirect->fd == redirect->fd)
			last_input_node = current_node;
		if (!current_node->next)
			break ;
		current_node = current_node->next;
	}
	return (last_input_node->content == redirect);
}

// <の処理
// @todo エラー時のexit処理
int	handle_input(t_redirects *redirect, bool is_last)
{
	int	fd;

	if (!(is_readable(redirect->target)))
	{
		print_error(redirect->target, EMESS_NO_PERM);
		return (1);
	}
	fd = open_file(redirect->target);
	if (!is_last)
		return (0);
	if (dup2(fd, redirect->fd) == -1)
		printf("dup2()");
	return (0);
}
