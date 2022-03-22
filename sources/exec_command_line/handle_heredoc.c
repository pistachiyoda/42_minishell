#include "minishell.h"

bool	is_end_heredoc(char *buf, char *target)
{
	int		buf_len;
	int		target_len;
	bool	is_same_text;

	if (!buf)
		return (true);
	buf_len = ft_strlen(buf);
	target_len = ft_strlen(target);
	is_same_text = ft_strncmp(buf, target, target_len) == 0;
	if (buf_len == target_len && is_same_text)
	{
		free(buf);
		return (true);
	}
	return (false);
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
		if (is_end_heredoc(buf, redirect->target))
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
			ft_putstr_fd("\n", 2);
		return (WTERMSIG(status) + 128);
	}
	return (1);
}

int	handle_redirect_nodes(t_list *redirect_node, t_environ *env,
	t_cmd_block	*cmd_block)
{
	t_redirects	*redirect;
	int			doc_pipe_fds[2];
	int			status;

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
	return (0);
}

int	handle_heredoc_input(t_environ *env, t_list *cmd_list)
{
	t_cmd_block	*cmd_block;
	t_list		*redirect_node;
	int			status;

	while (cmd_list)
	{
		cmd_block = (t_cmd_block *)cmd_list->content;
		redirect_node = cmd_block->redirects;
		while (redirect_node)
		{
			status = handle_redirect_nodes(redirect_node, env, cmd_block);
			if (status > 128)
				return (status);
			redirect_node = redirect_node->next;
		}
		cmd_list = cmd_list->next;
	}
	return (0);
}
