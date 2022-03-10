#include "minishell.h"

int	*dup_builtin_redirects(t_list *first_redirect)
{
	int		*fd_list;
	t_list	*current_redirect;
	int		i;

	current_redirect = first_redirect;
	fd_list = malloc(sizeof(int *) * ft_lstsize(first_redirect));
	i = 0;
	while (current_redirect)
	{
		fd_list[i] = dup(((t_redirects *)current_redirect->content)->fd);
		i ++;
		current_redirect = current_redirect->next;
	}
	return (fd_list);
}

int	run_builtin_only_command(t_list *cmd_list, t_environ *env)
{
	int		status;
	int		*fd_list;
	t_list	*first_redirect;
	t_list	*current_redirect;
	int		i;

	first_redirect = ((t_cmd_block *)cmd_list->content)->redirects;
	fd_list = dup_builtin_redirects(first_redirect);
	handle_heredoc_input(cmd_list);
	handle_redirects((t_cmd_block *)cmd_list->content);
	status = run_builtin_command((t_cmd_block *)cmd_list->content, env);
	close_doc_pipe_fd((t_cmd_block *)cmd_list->content);
	i = 0;
	current_redirect = first_redirect;
	while (current_redirect)
	{
		dup2_wrapper(fd_list[i],
			((t_redirects *)current_redirect->content)->fd);
		close(fd_list[i]);
		i ++;
		current_redirect = current_redirect->next;
	}
	free(fd_list);
	return (status);
}