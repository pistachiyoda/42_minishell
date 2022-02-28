#include "minishell.h"

bool	is_last_heredoc(t_redirects *redirect, t_list *redirects)
{
	t_list		*current_node;
	t_list		*last_input_node;
	t_redirects	*current_redirect;

	if (!redirects)
		return (false);
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
	if (!last_input_node)
		return (false);
	return (((t_redirects *)last_input_node->content)->redirect == HEREDOC);
}

void	handle_command_line(t_list *cmd_list, char **envp)
{
	int	pipe_fds[FD_MAX][2];

	if (handle_redirect((t_cmd_block *)cmd_list->content, pipe_fds) != 0)
		exit(1);
	exec_command(((t_cmd_block *)cmd_list->content)->command,
		((t_cmd_block *)cmd_list->content)->args, envp);
	exit(1);
}

int	exec_command_line(t_list *cmd_list, char **envp)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == 0)
		handle_command_line(cmd_list, envp);
	waitpid(pid, &status, 0);
	return (1);
}
