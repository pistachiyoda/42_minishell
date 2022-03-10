#include "minishell.h"

void	handle_unused_heredoc(char *str)
{
	free(str);
	exit(0);
}

void	flush_heredoc(char *str, int doc_pipe_fds[2])
{
	char	*tmp;

	tmp = str;
	close_wrapper(doc_pipe_fds[0]);
	if (write(doc_pipe_fds[1], str, ft_strlen(str)) == -1)
	{
		perror("write()");
		exit(1);
	}
	close_wrapper(doc_pipe_fds[1]);
	free(tmp);
	exit(0);
}

void	handle_each_input(char *limiter, bool is_last, int	doc_pipe_fds[2])
{
	char	*str;
	char	*buf;
	char	*tmp;

	str = ft_strdup("");
	if (str == NULL)
		exit(1);
	while (1)
	{
		buf = readline("> ");
		if ((ft_strlen(buf) == ft_strlen(limiter))
			&& ft_strncmp(buf, limiter, ft_strlen(limiter)) == 0)
			break ;
		tmp = str;
		str = ft_strjoin2(str, buf);
		free(buf);
		free(tmp);
	}
	if (!is_last)
		handle_unused_heredoc(str);
	flush_heredoc(str, doc_pipe_fds);
}

void	handle_heredoc(char *limiter, bool is_last, int doc_pipe_fds[2])
{
	int		pid;

	pipe_wrapper(doc_pipe_fds);
	pid = fork_wrapper();
	if (pid == 0)
		handle_each_input(limiter, is_last, doc_pipe_fds);
	close_wrapper(doc_pipe_fds[1]);
	waitpid_wrapper(pid, NULL, 0);
}

int	handle_heredoc_input(t_list *cmd_list)
{
	t_cmd_block	*cmd_block;
	t_list		*redirect_node;
	t_redirects	*redirect;
	int			doc_pipe_fds[2];

	while (cmd_list)
	{
		cmd_block = (t_cmd_block *)cmd_list->content;
		redirect_node = cmd_block->redirects;
		while (redirect_node)
		{
			redirect = redirect_node->content;
			if (redirect->redirect == HEREDOC)
			{
				handle_heredoc(redirect->target,
					is_last_fd_input_redirect(
						redirect, cmd_block->redirects),
					doc_pipe_fds);
				redirect->doc_fd = doc_pipe_fds[0];
			}
			redirect_node = redirect_node->next;
		}
		cmd_list = cmd_list->next;
	}
	return (0);
}
