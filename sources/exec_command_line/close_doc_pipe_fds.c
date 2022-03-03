#include "minishell.h"

int	close_doc_pipe_fds(int doc_pipe_fds[FD_MAX][2], t_cmd_block *cmd_block)
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
			close(doc_pipe_fds[redirect->fd][0]);
			close(doc_pipe_fds[redirect->fd][1]);
		}
		if (!redirect_node->next)
			break;
		redirect_node = redirect_node->next;
	}
	return (0);
}
