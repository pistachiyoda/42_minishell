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
	close(doc_pipe_fds[0]);
	write(doc_pipe_fds[1], str, ft_strlen(str));
	close(doc_pipe_fds[1]);
	free(tmp);
	exit(0);
}

void	handle_each_input(char *limiter, bool is_last, int	doc_pipe_fds[2])
{
	char	*str;
	char	*buf;
	char	*tmp;

	str = ft_strdup("");
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

	if (is_last)
	{
		if (pipe(doc_pipe_fds) == -1)
		{
			perror("pipe()");
			exit(1);
		}
	}
	pid = fork();
	if (pid == 0)
		handle_each_input(limiter, is_last, doc_pipe_fds);
	waitpid(pid, NULL, 0);
}

int	handle_heredoc_input(t_cmd_block *cmd_block, int	doc_pipe_fds_arr[FD_MAX][2])
{
	t_list		*redirect_node;
	t_redirects	*redirect;

	redirect_node = cmd_block->redirects;
	if (!redirect_node)
		return (0);
	while (1)
	{
		redirect = redirect_node->content;
		if (redirect->redirect == HEREDOC)
			handle_heredoc(
				redirect->target,
				is_last_fd_input_redirect(redirect, cmd_block->redirects),
				doc_pipe_fds_arr[redirect->fd]);
		if (redirect_node->next == NULL)
			break ;
		redirect_node = redirect_node->next;
	}
	return (0);
}
