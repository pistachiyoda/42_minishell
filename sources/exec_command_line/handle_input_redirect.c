#include "minishell.h"

bool	is_last_input_redirect(t_list *redirects, t_list *node)
{
	t_list		*current_node;
	t_list		*last_input_node;
	t_redirects	*current_redirect;

	current_node = redirects;
	last_input_node = NULL;
	while (1)
	{
		current_redirect = current_node->content;
		if (current_redirect->redirect == INPUT
			|| current_redirect->redirect == HEREDOC)
			last_input_node = current_node;
		if (!current_node->next)
			break ;
		current_node = current_node->next;
	}
	return (last_input_node == node);
}

// <の処理
// @todo エラー時のexit処理
void	handle_input(char *filename, bool is_last)
{
	int	fd;

	if (!(is_readable(filename)))
		exit(1);
	fd = open_file(filename);
	if (!is_last)
		return ;
	if (dup2(fd, 0) == -1)
		printf("dup2()");
}
