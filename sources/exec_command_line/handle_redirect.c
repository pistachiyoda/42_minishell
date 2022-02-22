#include "minishell.h"

bool	is_last_output_redirect(t_list *redirects, t_list *node)
{
	t_list		*current_node;
	t_list		*last_output_node;
	t_redirects	*current_redirect;

	current_node = redirects;
	last_output_node = NULL;
	while (1)
	{
		current_redirect = current_node->content;
		if (current_redirect->redirect == APPEND
			|| current_redirect->redirect == WRITE)
			last_output_node = current_node;
		if (!current_node->next)
			break ;
		current_node = current_node->next;
	}
	return (last_output_node == node);
}

// >, >>の処理
void	handle_output(char *filename, enum e_REDIRECTS	redirect, bool is_last)
{
	int	fd;

	if (!(is_writable(filename)))
		exit(0);
	if (redirect == WRITE)
		fd = open_or_create_file(filename, O_WRONLY | O_CREAT | O_TRUNC);
	if (redirect == APPEND)
		fd = open_or_create_file(filename, O_APPEND | O_WRONLY | O_CREAT);
	if (fd == -1)
		exit(1);
	if (!is_last)
		return ;
	if (dup2(fd, 1) == -1)
		exit(1);
}

int	handle_redirect(t_cmd_block *cmd_block, int	pipe_fds[2])
{
	t_list		*redirect_node;
	t_redirects	*redirect;

	redirect_node = cmd_block->redirects;
	if (!redirect_node)
		return (0);
	handle_heredoc_loop(cmd_block, pipe_fds);
	redirect_node = cmd_block->redirects;
	while (1)
	{
		redirect = redirect_node->content;
		if (redirect->redirect == INPUT)
			handle_input(redirect->target,
				is_last_input_redirect(cmd_block->redirects, redirect_node));
		if (redirect->redirect == WRITE || redirect->redirect == APPEND)
			handle_output(redirect->target, redirect->redirect,
				is_last_output_redirect(cmd_block->redirects, redirect_node));
		if (redirect_node->next == NULL)
			break ;
		redirect_node = redirect_node->next;
	}
	return (0);
}
