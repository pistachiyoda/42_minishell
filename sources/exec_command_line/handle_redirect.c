#include "minishell.h"

bool	is_last_fd_output_redirect(
		t_redirects *redirect, t_list *redirects)
{
	t_list		*current_node;
	t_list		*last_output_node;
	t_redirects	*current_redirect;

	current_node = redirects;
	last_output_node = NULL;
	while (1)
	{
		current_redirect = current_node->content;
		if ((current_redirect->redirect == APPEND
				|| current_redirect->redirect == WRITE)
			&& current_redirect->fd == redirect->fd)
			last_output_node = current_node;
		if (!current_node->next)
			break ;
		current_node = current_node->next;
	}
	return (last_output_node->content == redirect);
}

// >, >>の処理
// @todo:exitはreturnに変更
int	handle_output(t_redirects *redirect, bool is_last)
{
	int	fd;

	if (is_exists(redirect->target) && is_directory(redirect->target))
	{
		print_error(redirect->target, EMESS_IS_DIR2);
		return (1);
	}
	if (!(is_writable(redirect->target)))
	{
		print_error(redirect->target, EMESS_NO_PERM);
		return (1);
	}
	if (redirect->redirect == WRITE)
		fd = open_or_create_file(
				redirect->target, O_WRONLY | O_CREAT | O_TRUNC);
	if (redirect->redirect == APPEND)
		fd = open_or_create_file(
				redirect->target, O_APPEND | O_WRONLY | O_CREAT);
	if (fd == -1)
		exit(1);
	if (!is_last)
		return (0);
	dup2_wrapper(fd, redirect->fd);
	return (0);
}

// HEREDOCの場合は、inputのラストだったらパイプに書き込まれた内容をdup
// INPUTの場合はファイル処理＋inputのラストだったらredirect->fdでdup
// WRITEの場合はファイル処理＋outpuのラストだったらredirect->fdでdup
int	handle_redirect(t_redirects	*redirect, t_cmd_block *cmd_block)
{
	if (redirect->redirect == HEREDOC)
	{
		if (is_last_fd_input_redirect(redirect, cmd_block->redirects))
			dup2_wrapper(redirect->doc_fd, redirect->fd);
		close_wrapper(redirect->doc_fd);
	}
	if (redirect->redirect == INPUT)
	{
		if (handle_input(redirect,
				is_last_fd_input_redirect(
					redirect, cmd_block->redirects)) != 0)
			return (1);
	}
	if (redirect->redirect == WRITE || redirect->redirect == APPEND)
	{
		if (handle_output(redirect,
				is_last_fd_output_redirect(
					redirect, cmd_block->redirects)) != 0)
			return (1);
	}
	return (0);
}

void	handle_redirects(t_cmd_block *cmd_block)
{
	t_list		*redirect_node;
	t_redirects	*redirect;
	int			ret;

	redirect_node = cmd_block->redirects;
	while (1)
	{
		if (!redirect_node)
			break ;
		redirect = redirect_node->content;
		ret = handle_redirect(redirect, cmd_block);
		if (ret != 0)
			exit(ret);
		if (redirect_node->next == NULL)
			break ;
		redirect_node = redirect_node->next;
	}
}
