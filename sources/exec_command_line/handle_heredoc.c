#include "minishell.h"

char	*ft_strjoin2(char const *s1, char const *s2)
{
	size_t	len;
	char	*str;
	char	*ret;

	len = ft_strlen((char *)s1) + ft_strlen((char *)s2);
	str = (char *)malloc(sizeof(char) * (len + 2));
	if (str == NULL)
		return (NULL);
	ret = str;
	while (*s1 != '\0')
		*str++ = *s1++;
	while (*s2 != '\0')
		*str++ = *s2++;
	*str++ = '\n';
	*str = '\0';
	return (ret);
}

void	handle_unused_heredoc(char *str, int pipe_fds[2])
{
	free(str);
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	exit(0);
}

void	flush_heredoc(char *str, int pipe_fds[2])
{
	char	*tmp;

	tmp = str;
	close(pipe_fds[0]);
	write(pipe_fds[1], str, ft_strlen(str));
	close(pipe_fds[1]);
	free(tmp);
	exit(0);
}

void	handle_heredoc(char *limiter, bool is_last, int	pipe_fds[2])
{
	char	*buf;
	char	*str;
	char	*tmp;
	int		pid;

	pid = fork();
	str = ft_strdup("");
	if (pid == 0)
	{
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
			handle_unused_heredoc(str, pipe_fds);
		flush_heredoc(str, pipe_fds);
	}
	waitpid(pid, NULL, 0);
}

void	handle_heredoc_loop(t_cmd_block *cmd_block, int	pipe_fds[2])
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
				is_last_input_redirect(
					(t_redirects *)cmd_block->redirects, redirect_node),
				pipe_fds);
		if (redirect_node->next == NULL)
			break ;
		redirect_node = redirect_node->next;
	}
}
