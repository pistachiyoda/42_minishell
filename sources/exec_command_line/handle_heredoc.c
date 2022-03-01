#include "minishell.h"

void	handle_unused_heredoc(char *str)
{
	free(str);
	exit(0);
}

void	flush_heredoc(char *str, int pipe_a[2])
{
	char	*tmp;

	tmp = str;
	close(pipe_a[0]);
	write(pipe_a[1], str, ft_strlen(str));
	close(pipe_a[1]);
	free(tmp);
	exit(0);
}

void	handle_each_input(char *limiter, bool is_last, int	pipe_a[2])
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
	flush_heredoc(str, pipe_a);
}

void	handle_heredoc(char *limiter, bool is_last, int pipe_a[2])
{
	int		pid;

	if (is_last)
	{
		if (pipe(pipe_a) == -1)
		{
			perror("pipe()");
			exit(1);
		}
	}
	pid = fork();
	if (pid == 0)
		handle_each_input(limiter, is_last, pipe_a);
	waitpid(pid, NULL, 0);
}

void	handle_heredoc_loop(t_cmd_block *cmd_block, int	pipe_a[FD_MAX][2])
{
	t_list		*redirect_node;
	t_redirects	*redirect;

	redirect_node = cmd_block->redirects;
	while (1)
	{
		redirect = redirect_node->content;
		if (redirect->redirect == HEREDOC)
			handle_heredoc(
				redirect->target,
				is_last_input_redirect(redirect, cmd_block->redirects),
				pipe_a[redirect->fd]);
		if (redirect_node->next == NULL)
			break ;
		redirect_node = redirect_node->next;
	}
}
