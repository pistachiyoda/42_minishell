#include "minishell.h"

int	close_doc_pipe_fd(t_cmd_block *cmd_block)
{
	t_list		*redirect_node;
	t_redirects	*redirect;

	if (!cmd_block->redirects)
		return (0);
	redirect_node = cmd_block->redirects;
	while (1)
	{
		redirect = redirect_node->content;
		if (redirect->redirect == HEREDOC
			&& is_last_fd_input_redirect(redirect, cmd_block->redirects))
		{
			close_wrapper(redirect->doc_fd);
		}
		if (!redirect_node->next)
			break ;
		redirect_node = redirect_node->next;
	}
	return (0);
}
