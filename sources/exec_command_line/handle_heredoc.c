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

void	handle_each_input(
		t_environ *env, t_redirects *redirect,
		bool is_last, int doc_pipe_fds[2])
{
	char	*str;
	char	*buf;
	char	*expanded_buf;

	str = ft_xstrdup("", "handle_each_input");
	while (1)
	{
		buf = readline("> ");
		if (!buf || ((ft_strlen(buf) == ft_strlen(redirect->target))
				&& ft_strncmp(buf, redirect->target,
					ft_strlen(redirect->target)) == 0))
			break ;
		if (redirect->redirect == HEREDOC)
		{
			expanded_buf = expand_env_variables_in_buf(env, buf);
			str = ft_xstrjoin2_with_free(
					str, expanded_buf, "handle_each_input");
			free(buf);
		}
		else
			str = ft_xstrjoin2_with_free(str, buf, "handle_each_input");
	}
	if (!is_last)
		handle_unused_heredoc(str);
	flush_heredoc(str, doc_pipe_fds);
}

int	handle_heredoc(
		t_environ *env, t_redirects *redirect,
		bool is_last, int doc_pipe_fds[2])
{
	int		pid;
	int		status;

	pipe_wrapper(doc_pipe_fds);
	set_signal(SIG_IGN, SIG_IGN);
	pid = fork_wrapper();
	if (pid == 0)
	{
		set_signal(SIG_DFL, SIG_IGN);
		handle_each_input(env, redirect, is_last, doc_pipe_fds);
	}
	close_wrapper(doc_pipe_fds[1]);
	waitpid_wrapper(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		return (WTERMSIG(status) + 128);
	}
	return (1);
}

int	handle_heredoc_input(t_environ *env, t_list *cmd_list)
{
	t_cmd_block	*cmd_block;
	t_list		*redirect_node;
	t_redirects	*redirect;
	int			doc_pipe_fds[2];
	int			status;

	while (cmd_list)
	{
		cmd_block = (t_cmd_block *)cmd_list->content;
		redirect_node = cmd_block->redirects;
		while (redirect_node)
		{
			redirect = redirect_node->content;
			if (redirect->redirect == HEREDOC
				|| redirect->redirect == QUOTED_HEREDOC)
			{
				status = handle_heredoc(env, redirect,
						is_last_fd_input_redirect(redirect,
							cmd_block->redirects), doc_pipe_fds);
				if (status > 128)
					return (status);
				set_signal(sigint_handler, SIG_IGN);
				redirect->doc_fd = doc_pipe_fds[0];
			}
			redirect_node = redirect_node->next;
		}
		cmd_list = cmd_list->next;
	}
	return (0);
}
